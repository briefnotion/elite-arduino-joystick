Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Arduino USB Thumb Joystick: An Auxiliary Thrust Input Solution

## Executive Summary

This initiative delivers a lightweight, cost-effective, single-board peripheral
that interfaces with an Arduino Leonardo/Micro to provide two (2) incremental
axes of analog control and one (1) discrete button input, surfaced to the host
operating system as a standards-compliant USB Human Interface Device (HID)
joystick. No proprietary drivers are required, ensuring a frictionless
onboarding experience across supported platforms.

## Background & Strategic Rationale

This solution was originally scoped and delivered in support of **Elite
Dangerous**, to close a well-documented capability gap: legacy HAT switches
provide only four (4) binary directional states, whereas mission-critical
vertical and lateral thrust control requires true analog granularity. The
solution has since been successfully redeployed into a second vertical,
**Star Citizen**, without any re-engineering - because the device presents to
the operating system as a generic USB joystick, it is inherently
game-agnostic and portable to any downstream application accepting joystick
axis input. When paired with a primary flight stick that already services
roll/pitch/yaw, this solution enables a complete analog strafe capability
(vertical and lateral) at low incremental cost.

## Key Capabilities

- Two (2) analog axes and one (1) button, delivered via a standards-based USB
  HID joystick interface.
- Automated, self-calibrating center-point determination at startup, with
  per-axis inversion configurability to support diverse hardware orientations.
- Recurring recalibration during idle-state windows, mitigating long-term
  signal drift (attributable to thermal variance, humidity, and component
  settling in lower-cost potentiometers) without requiring a device restart.
  Recalibration is strictly gated on the device being both stationary *and*
  resting near center, ensuring zero impact to the active-use experience.
- Adaptive deadzone sizing, calibrated against observed real-world signal
  noise rather than a static assumption, with a conservative, gradual
  reduction methodology to avoid overcorrection on any single sampling
  window.
- Optional dual-slope precision response curve and edge-snap-to-maximum
  functionality, both disabled by default in favor of a linear response
  baseline.
- Status LED indicator and an opt-in live Serial diagnostic output stream,
  supporting rapid tuning and validation cycles.
- All configurable parameters are centralized as top-of-file constants,
  minimizing the need for stakeholders to engage with underlying
  implementation logic.

## Hardware Requirements

- An Arduino board with native USB HID support - **Leonardo**, **Micro**, or
  **Pro Micro** (ATmega32u4). Boards without native USB capability (e.g. Uno,
  Nano) are explicitly out of scope and unsupported.
- A two-axis analog joystick module (dual potentiometer assembly plus an
  integrated push-button).

### Wiring / Interconnect Specification

| Joystick Pin | Arduino Pin | Notes |
|---|---|---|
| VCC | 5V or 3.3V | Reference `VOLTAGE_SCALE` below - must align with actual power configuration |
| GND | GND | |
| X axis (VRx) | A0 | `PIN_AXIS_X` |
| Y axis (VRy) | A1 | `PIN_AXIS_Y` |
| Button (SW) | 6 | `PIN_BUTTON` - leverages the internal pull-up resistor; no external component required |

Note: if the joystick module is provisioned from a 3.3V source rather than
5V, its analog output will only utilize a subset of the ADC's available
range. The `VOLTAGE_SCALE` constant compensates for this discrepancy
(`1.0` for 5V, `1.5151` for 3.3V). Misconfiguration of this value will
result in persistent near-edge axis readings and a degraded end-user
experience.

## Software Dependencies

- [Arduino IDE](https://www.arduino.cc/en/software) (or `arduino-cli`), with
  the **Arduino AVR Boards** package installed as a prerequisite.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary),
  authored by Matthew Heironimus - deployable via *Sketch > Include Library >
  Manage Libraries...* (query "Joystick"), or alternatively sourced directly
  from the linked repository and integrated via *Sketch > Include Library >
  Add .ZIP Library...*.

### Build & Deployment Procedure

1. Open `EDJoystick/EDJoystick.ino` within the Arduino IDE.
2. Navigate to **Tools > Board > Arduino Leonardo** (or Micro/Pro Micro, per
   your deployed hardware configuration).
3. Select the appropriate **Tools > Port**.
4. Execute **Upload**.

Upon successful deployment, the board will be recognized by the host
operating system as a generic USB joystick with zero additional driver
overhead. From that point forward, it may be mapped to any axis/button
binding within your game or flight-stick management software, consistent
with the solution's stated game-agnostic design principles.

## Configuration Management

All parameters anticipated to require end-user adjustment are consolidated
within the `USER CONFIGURATION` block at the top of `EDJoystick.ino` - pin
assignments, axis inversion, response curve behavior, deadzone/recalibration
tuning, and the diagnostic output toggle are all exposed as clearly
annotated constants. Of particular note:

- **`VIEW_DATA`** - toggle to `true` and redeploy to enable a real-time
  diagnostic stream over Serial (115200 baud), surfacing raw/processed axis
  values, current deadzone sizing, button state, and calibration events.
  Recommended for validation prior to finalizing tuning changes. Revert to
  `false` for standard operational use to minimize overhead.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - govern the cadence and threshold criteria
  under which the controller autonomously re-centers itself during idle
  periods.

No redeployment is required to support ongoing day-to-day operational use -
calibration and deadzone tuning are executed automatically at startup and
at recurring intervals thereafter, with no end-user intervention required.

## Acknowledgments

- Built on the [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary),
  authored by Matthew Heironimus.
- Foundational reference / original approach: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), informed by a contribution from Goustoulos.

## Licensing

Distributed under the GNU General Public License v3.0 - refer to
[LICENSE](LICENSE) for full terms and conditions.
