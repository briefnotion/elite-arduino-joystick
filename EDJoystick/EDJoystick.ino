// ***************************************************************************************
// *
// * Core      | Everything within this document is proprietary to Core Dynamics.
// * Dynamics  | Any unauthorized duplication will be subject to prosecution.
// *
// * Department : (R+D)^2
// * Sub Dept: Programming
// * Location ID: 856-45B
// * (c) 2856 - 2857 Core Dynamics
// ***************************************************************************************
// * PROJECTID: /*+T,&j<\&59    Revision: 00000002.12A
// *
// * Dependencies:
// * - Joystick.h (V2.0) by Matthew Heironimus
// * Source: https://github.com/MHeironimus/ArduinoJoystickLibrary
// *
// * Rev 00000002.12A
// *   - Fixed: holding the stick steadily off-center could get adopted as
// *     the new center by periodic recalibration, since "idle" only checked
// *     for no change, not nearness to center. Recalibration now also
// *     requires the stick to be resting within RECALIBRATION_CENTER_TOLERANCE
// *     deadzone-widths of center.
// *
// * Rev 00000002.11A
// *   - Added VIEW_DATA debug toggle: prints live axis/deadzone/button
// *     values plus calibration results to Serial when enabled.
// *
// * Rev 00000002.10A
// *   - Deadzone is now self-tuning: grows instantly on new jitter, eases
// *     down gradually otherwise, floored at DEADZONE_MINIMUM. Combines
// *     with periodic recalibration to shrink the deadzone over time on
// *     cheap/high-tolerance sticks without reintroducing drift.
// *
// * Rev 00000002.09A
// *   - Added periodic recalibration: the controller will now recenter
// *     itself while idle (see ENABLE_PERIODIC_RECALIBRATION) instead of
// *     only calibrating once at boot.
// *
// * Rev 00000002.08A
// *   - Readability pass: user-configurable values collected into one block up
// *     top, functions documented, dead code and unused variables removed.
// *     No behavior change from 00000002.07A.
// ***************************************************************************************

#include "Joystick.h"

/* =========================================================================
   USER CONFIGURATION
   Everything a modder is likely to want to change lives here. Nothing
   below this block needs to be touched for normal tuning.
   ========================================================================= */

// --- Pins ---
const int PIN_AXIS_X     = A0;
const int PIN_AXIS_Y     = A1;
const int PIN_BUTTON     = 6;
const int PIN_STATUS_LED = LED_BUILTIN;

// --- Power ---
// The joystick's raw 0-1023 analog reading is multiplied by this before use.
// Set to 1.0 if the joystick is wired to 5V, or 1.5151 if wired to 3.3V.
const float VOLTAGE_SCALE = 1.5151;

// --- Axis Direction ---
// Flip an axis if pushing the stick moves the reported value the wrong way.
const bool INVERT_X = false;
const bool INVERT_Y = true;

// --- Response Curve ---
// When ENABLE_PRECISION_CURVE is false, axis output is linear (raw position,
// scaled by LINEAR_SCALE). When true, a dual-slope curve is used instead:
// fine control near center (CURVE_INNER_SLOPE) and coarser control beyond
// CURVE_BREAKPOINT (CURVE_OUTER_SLOPE). Both are compile-time options.
const bool  ENABLE_PRECISION_CURVE = false;
const int   CURVE_INNER_SLOPE      = 4;    // e.g. 4 = quarter-speed near center
const int   CURVE_OUTER_SLOPE      = 2;    // e.g. 2 = half-speed past the breakpoint
const int   CURVE_BREAKPOINT       = 256;  // |value| (0-512) where the slope switches
const float LINEAR_SCALE           = 1.0;  // divisor applied when the curve is off

// Snap to full deflection (+-512) once the stick is within EDGE_SNAP_MARGIN
// of its limit. Applies whether or not the precision curve is enabled.
const bool ENABLE_EDGE_SNAP = false;
const int  EDGE_SNAP_MARGIN = 12;

// --- Button Behavior ---
// false: PIN_BUTTON reports as a normal joystick fire button.
// true:  PIN_BUTTON instead toggles "half precision" mode on press, which
//        halves both axes' output for finer control (see loop() step 5).
const bool BUTTON_TOGGLES_HALF_PRECISION = false;

// --- Deadzone Self-Tuning ---
// Cheap sticks can have a lot of center jitter, so the deadzone is measured
// during calibration rather than fixed. It grows immediately if a
// calibration observes more jitter than it currently covers (drift safety
// first), but only eases downward gradually across repeated calibrations -
// see calibrateCenter() - so a single unusually quiet sample window can't
// snap it too tight. Higher DEADZONE_SHRINK_DIVISOR = more cautious/slower
// shrinking.
const int DEADZONE_SHRINK_DIVISOR = 4;
const int DEADZONE_MINIMUM        = 2;

// --- Timing ---
// Delay (ms) between read/report cycles, and between calibration samples at boot.
const int LOOP_INTERVAL_MS = 15;

// --- Debug Output ---
// When true, prints the live raw/processed axis values, deadzone, button
// state, and calibration results to Serial once per loop, so you can watch
// what the controller is doing (e.g. in the Arduino IDE's Serial Monitor).
// Flip back to false once you're happy with the tuning - no need to remove
// the code.
const bool VIEW_DATA      = false;
const long VIEW_DATA_BAUD = 115200;

// --- Periodic Recalibration ---
// The initial boot-time calibration can go stale (temperature, humidity, the
// stick just settling with use). When enabled, the controller will quietly
// recenter itself using the same logic as the boot calibration, but only
// while the stick is idle - never while it's actively being moved.
const bool ENABLE_PERIODIC_RECALIBRATION = true;

// How long the reported position must go unchanged before the stick is
// considered "at rest" and eligible for recalibration.
const unsigned long RECALIBRATION_IDLE_MS = 3000UL;       // 3 seconds

// Minimum time between recalibrations, even if idle the whole time.
const unsigned long RECALIBRATION_INTERVAL_MS = 300000UL; // 5 minutes

// A calibration is skipped if the stick is currently resting more than this
// many deadzone-widths away from center - e.g. held off to one side and left
// there. Without this, "unchanged for RECALIBRATION_IDLE_MS" alone can't
// tell "parked off-center" apart from "genuinely at rest."
const int RECALIBRATION_CENTER_TOLERANCE = 2; // e.g. 2 = must be within 2x deadZone of center

/* =========================================================================
   Joystick Device
   Structural setup (button/axis count) - edit here if adding more buttons
   or axes, alongside the matching handling code in loop().
   ========================================================================= */

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  1, 0,                  // 1 button, 0 hats
  true, true, false,     // X, Y enabled, Z disabled
  false, false, false,   // Rx, Ry, Rz disabled
  false, false,          // Rudder, Throttle disabled
  false, false, false    // Accelerator, Brake, Steering disabled
);

/* =========================================================================
   Runtime State (internal - no need to edit)
   ========================================================================= */

// Learned at boot by calibrateCenter(), to correct for a stick that doesn't
// rest exactly at the analog midpoint.
int xCenterOffset = 0;
int yCenterOffset = 0;

// Deadzone radius; starts at a small fallback and is self-tuned by
// calibrateCenter() to the smallest value that still covers the stick's
// observed rest jitter (see DEADZONE_SHRINK_DIVISOR).
int deadZone = 5;

// 3-sample rolling average buffers, indexed by sampleIndex (see readSmoothed).
int xSamples[3] = {0, 0, 0};
int ySamples[3] = {0, 0, 0};
int sampleIndex = 0;

// Last values actually sent to the host, so we only transmit on real change.
int lastReportedX = -1;
int lastReportedY = -1;
int lastButtonPin = HIGH;

// Current half-precision state, toggled by the button when
// BUTTON_TOGGLES_HALF_PRECISION is enabled.
bool halfPrecisionActive = false;

// Status LED animation phase: 0 = idle/steady, 2 = settle-back-to-steady is
// due next frame. See loop() step 6 for the full explanation.
int ledPhase = 0;

// Timestamps used to decide when it's safe to recalibrate (see loop() step 11).
unsigned long lastMovementMillis    = 0;
unsigned long lastCalibrationMillis = 0;

// Tracks whether we've already printed a "skipped, off-center" debug line
// for the current idle stretch, so it logs once instead of every frame.
bool recalSkipAnnounced = false;

/* =========================================================================
   Helpers
   ========================================================================= */

// Returns -1, 0, or +1 for the sign of val. (0 stays 0 rather than dividing by it.)
float signOf(float val) {
  if (val > 0) return 1.0;
  if (val < 0) return -1.0;
  return 0.0;
}

// Takes one new raw reading into the rolling buffer and returns the
// 3-sample average, to smooth out single-read analog noise.
float readSmoothed(int pin, int sampleSet[], int index) {
  sampleSet[index] = analogRead(pin) * VOLTAGE_SCALE;

  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum += sampleSet[i];
  }
  return (float)sum / 3.0f;
}

// Clamps a value to the joystick's +-512 reporting range.
float clampToRange(float val) {
  if (val < -512.0) return -512.0;
  if (val >  512.0) return  512.0;
  return val;
}

// Dual-slope precision curve: fine control near center, coarser control past
// CURVE_BREAKPOINT, optionally snapped to full deflection near the edge.
float applyPrecisionCurve(float val) {
  float absVal = abs(val);
  float sign = signOf(val);

  if (ENABLE_EDGE_SNAP && absVal > (512 - EDGE_SNAP_MARGIN)) {
    return sign * 512;
  }
  if (absVal < CURVE_BREAKPOINT) {
    return val / CURVE_INNER_SLOPE;
  }
  return (val / CURVE_OUTER_SLOPE) - (sign * (CURVE_BREAKPOINT / CURVE_INNER_SLOPE));
}

// Straight-line response (scaled by LINEAR_SCALE), with the same optional edge snap.
float applyLinear(float val) {
  float absVal = abs(val);
  float sign = signOf(val);

  if (ENABLE_EDGE_SNAP && absVal > (512 - EDGE_SNAP_MARGIN)) {
    return sign * 512;
  }
  return val / LINEAR_SCALE;
}

// Samples both axes for a short window to find their true center and how
// much they drift/jitter while at rest, so the deadzone can absorb that.
// `reason` is just a label for the VIEW_DATA log (e.g. "BOOT" or "IDLE").
void calibrateCenter(const char* reason) {
  int previousDeadZone = deadZone;

  float xMin = 9999, xMax = -9999;
  float yMin = 9999, yMax = -9999;

  for (int i = 0; i < 20; i++) {
    float xErr = 512.0 - analogRead(PIN_AXIS_X) * VOLTAGE_SCALE;
    float yErr = 512.0 - analogRead(PIN_AXIS_Y) * VOLTAGE_SCALE;

    xMin = min(xMin, xErr);
    xMax = max(xMax, xErr);
    yMin = min(yMin, yErr);
    yMax = max(yMax, yErr);

    delay(LOOP_INTERVAL_MS);
  }

  xCenterOffset = (xMin + xMax) / 2;
  yCenterOffset = (yMin + yMax) / 2;

  int observedJitter = (int)(max(xMax - xMin, yMax - yMin)) + 2;

  if (observedJitter > deadZone) {
    // Worse jitter than we've been covering - adopt it right away.
    deadZone = observedJitter;
  } else {
    // Quieter than before - ease toward it instead of snapping down, so one
    // lucky quiet sample window can't make the deadzone too tight.
    int step = (deadZone - observedJitter) / DEADZONE_SHRINK_DIVISOR;
    if (step < 1) step = 1;
    deadZone -= step;
    if (deadZone < observedJitter) deadZone = observedJitter;
  }

  if (deadZone < DEADZONE_MINIMUM) {
    deadZone = DEADZONE_MINIMUM;
  }

  if (VIEW_DATA) {
    Serial.print(F("[CALIBRATE:"));
    Serial.print(reason);
    Serial.print(F("] xOffset="));
    Serial.print(xCenterOffset);
    Serial.print(F(" yOffset="));
    Serial.print(yCenterOffset);
    Serial.print(F(" deadZone="));
    Serial.print(previousDeadZone);
    Serial.print(F(" -> "));
    Serial.println(deadZone);
  }
}

/* =========================================================================
   Setup / Loop
   ========================================================================= */

void setup() {
  if (VIEW_DATA) {
    Serial.begin(VIEW_DATA_BAUD);
    // Give the Serial Monitor a few seconds to connect so the boot
    // calibration line below isn't lost. Bounded, so normal use (and any
    // run without a monitor attached) is never held up for long.
    unsigned long serialWaitStart = millis();
    while (!Serial && (millis() - serialWaitStart) < 3000UL) {
      ; // waiting for the host to open the port
    }
  }

  Joystick.begin(false);  // manual sendState() - only transmit on real changes
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_STATUS_LED, OUTPUT);

  Joystick.setXAxis(512);
  Joystick.setYAxis(512);
  Joystick.sendState();

  calibrateCenter("BOOT");

  // Prime the rolling-average buffers so the first loop() reads aren't
  // skewed toward zero by empty buffer slots.
  for (int i = 0; i < 3; i++) {
    readSmoothed(PIN_AXIS_X, xSamples, sampleIndex);
    readSmoothed(PIN_AXIS_Y, ySamples, sampleIndex);
    sampleIndex = (sampleIndex + 1) % 3;
    delay(LOOP_INTERVAL_MS);
  }

  lastMovementMillis    = millis();
  lastCalibrationMillis = millis();
}

void loop() {
  // 1. Read + smooth both axes.
  float rawX = readSmoothed(PIN_AXIS_X, xSamples, sampleIndex);
  float rawY = readSmoothed(PIN_AXIS_Y, ySamples, sampleIndex);
  sampleIndex = (sampleIndex + 1) % 3;

  // 2. Center, tune, and invert.
  float x = rawX - 512 + xCenterOffset;
  float y = rawY - 512 + yCenterOffset;
  if (INVERT_X) x = -x;
  if (INVERT_Y) y = -y;

  // Raw offset from center, before the deadzone zeroes small values out -
  // kept around for the recalibration center-check in step 11.
  float centeredX = x;
  float centeredY = y;

  // 3. Deadzone: snap small jitter around center to exactly zero.
  if (abs(x) < deadZone) x = 0;
  if (abs(y) < deadZone) y = 0;

  // 4. Response curve, then clamp to the valid reporting range.
  if (ENABLE_PRECISION_CURVE) {
    x = applyPrecisionCurve(x);
    y = applyPrecisionCurve(y);
  } else {
    x = applyLinear(x);
    y = applyLinear(y);
  }
  x = clampToRange(x);
  y = clampToRange(y);

  // 5. Half-precision mode, if the button is toggling it.
  if (halfPrecisionActive) {
    x *= 0.5f;
    y *= 0.5f;
  }

  // 6. Map back to the joystick's 0-1023 HID range.
  int reportX = (int)x + 512;
  int reportY = (int)y + 512;
  int buttonPin = digitalRead(PIN_BUTTON);

  // 7. Stage the axes for transmission only if they actually moved.
  bool changed = false;
  if (reportX != lastReportedX || reportY != lastReportedY) {
    Joystick.setXAxis(reportX);
    Joystick.setYAxis(reportY);
    lastReportedX = reportX;
    lastReportedY = reportY;
    changed = true;
  }

  // 8. Button: either a normal fire button, or a half-precision toggle.
  if (buttonPin != lastButtonPin) {
    if (BUTTON_TOGGLES_HALF_PRECISION) {
      lastButtonPin = buttonPin;
      if (buttonPin == LOW) {  // active-low pullup: toggle on press, not release
        halfPrecisionActive = !halfPrecisionActive;
        changed = true;
      }
    } else {
      Joystick.setButton(0, buttonPin == LOW);  // active-low pullup
      lastButtonPin = buttonPin;
      changed = true;
    }
  }

  // 9. Transmit only when something actually changed.
  if (changed) {
    Joystick.sendState();
  }

  // 10. Status LED: steady state reflects halfPrecisionActive (on/off), and
  //     blips to the opposite value for one frame whenever a report was
  //     just sent, then settles back to steady on the following frame.
  //     A fresh change always takes priority over a pending settle.
  if (changed) {
    digitalWrite(PIN_STATUS_LED, halfPrecisionActive ? LOW : HIGH);
    ledPhase = 2;
  } else if (ledPhase == 2) {
    digitalWrite(PIN_STATUS_LED, halfPrecisionActive ? HIGH : LOW);
    ledPhase = 0;
  }

  // 11. Periodic recalibration: track how long it's been since anything
  //     changed (axis or button). If the stick has sat idle long enough,
  //     and enough time has passed since the last calibration, recenter.
  //     "Idle" is judged by the report going unchanged, not by it reading
  //     exactly center - that way a stick that has drifted off center while
  //     genuinely at rest can still be caught and corrected.
  if (changed) {
    lastMovementMillis = millis();
    recalSkipAnnounced = false;
  }

  if (ENABLE_PERIODIC_RECALIBRATION) {
    unsigned long idleFor = millis() - lastMovementMillis;
    unsigned long sinceLastCalibration = millis() - lastCalibrationMillis;

    if (idleFor >= RECALIBRATION_IDLE_MS && sinceLastCalibration >= RECALIBRATION_INTERVAL_MS) {
      int recalTolerance = deadZone * RECALIBRATION_CENTER_TOLERANCE;
      bool nearCenter = abs(centeredX) <= recalTolerance && abs(centeredY) <= recalTolerance;

      if (nearCenter) {
        if (VIEW_DATA) {
          Serial.print(F("[RECAL] idle for "));
          Serial.print(idleFor);
          Serial.println(F("ms, recalibrating..."));
        }
        calibrateCenter("IDLE");
        lastCalibrationMillis = millis();
      } else if (VIEW_DATA && !recalSkipAnnounced) {
        Serial.print(F("[RECAL] skipped - resting off-center (X="));
        Serial.print(centeredX, 1);
        Serial.print(F(" Y="));
        Serial.print(centeredY, 1);
        Serial.print(F(", tolerance=+-"));
        Serial.print(recalTolerance);
        Serial.println(F(")"));
        recalSkipAnnounced = true;
      }
    }
  }

  // 12. Live debug dump - see VIEW_DATA above.
  if (VIEW_DATA) {
    Serial.print(F("X raw="));
    Serial.print(rawX, 1);
    Serial.print(F(" out="));
    Serial.print(reportX);
    Serial.print(F(" | Y raw="));
    Serial.print(rawY, 1);
    Serial.print(F(" out="));
    Serial.print(reportY);
    Serial.print(F(" | deadZone="));
    Serial.print(deadZone);
    Serial.print(F(" | btn="));
    Serial.print(buttonPin);
    Serial.print(F(" | halfPrecision="));
    Serial.print(halfPrecisionActive);
    Serial.print(F(" | sent="));
    Serial.println(changed);
  }

  delay(LOOP_INTERVAL_MS);
}
