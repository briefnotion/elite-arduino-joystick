Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# NEURAL THUMB-INTERFACE // Auxiliary Vector-Thrust Rig

Jack in, choom. Bolted to the guts of an Arduino Leonardo/Micro, this
palm-sized analog nub wires straight into your rig's USB port and lights up
in the OS as just another joystick - two axes, one trigger-finger button,
zero fuss, zero drivers, zero corpo bloatware.

This chunk of silicon was first soldered together for **Elite Dangerous**,
back when a HAT switch's pathetic four-way binary click was all the vertical
and lateral thrust the grid had to offer. Not enough. This rig gives you the
real thing - true analog burn, up/down, left/right, no compromise. Since
then the build's jacked itself into a second cockpit, **Star Citizen**,
without a single line rewritten - because to the machine, it's just a
generic USB stick. No game owns it. Anything on the net that reads a
joystick axis, this thing talks to. Bolt it next to a flight stick already
running roll/pitch/yaw, and suddenly you've got full analog strafe -
vertical, lateral, all of it - burning on your six.

## Loadout

- Two analog axes + one button, broadcasting clean over standard USB HID -
  no proprietary handshake required.
- Self-calibrating center-lock on boot. Flip either axis if the signal's
  running backwards through the wire.
- Runs silent recalibration passes while the stick sits dormant, purging
  drift caused by heat, humidity, cheap pots settling into their groove -
  no reboot, no downtime. The rig only pulls this trick when the stick's
  gone still *and* parked near center. Never mid-run. Never mid-combat.
- Adaptive deadzone: the firmware watches the real noise floor instead of
  trusting some hardcoded guess, and tightens the window down slow, cycle
  by cycle, once it's sure. A junk stick doesn't get stuck wearing a permanent
  dead spot.
- Dual-slope precision curve and edge-snap-to-max are baked in and ready to
  flip on - dormant by default, running clean linear response until you say
  otherwise.
- Status LED pulses feedback in real time, and an optional live Serial
  data-feed lets you watch the raw telemetry stream while you tune.
- Every parameter that matters lives in one block at the top of the source -
  no need to crack open the core logic to retune your rig.

## Hardware Manifest

- An Arduino board running native USB HID - **Leonardo**, **Micro**, or
  **Pro Micro** (ATmega32u4). Boards without native USB (Uno, Nano) can't
  run this firmware. Don't even try jacking one in.
- A 2-axis analog joystick module - dual potentiometers plus a click-button
  trigger.

### Hardwire Schema

| Joystick Pin | Arduino Pin | Notes |
|---|---|---|
| VCC | 5V or 3.3V | See `VOLTAGE_SCALE` below - must match your actual power rail |
| GND | GND | |
| X axis (VRx) | A0 | `PIN_AXIS_X` |
| Y axis (VRy) | A1 | `PIN_AXIS_Y` |
| Button (SW) | 6 | `PIN_BUTTON` - runs on the internal pull-up, no extra resistor needed |

Run the joystick module off 3.3V instead of 5V and its analog signal only
sweeps part of the ADC's full range - `VOLTAGE_SCALE` in the firmware
compensates (`1.0` for 5V, `1.5151` for 3.3V). Get it wrong and your axes
flatline near one edge, permanently pegged like a fried neural port.

## Software Stack

- [Arduino IDE](https://www.arduino.cc/en/software) (or `arduino-cli` for
  the terminal-jockeys), with the **Arduino AVR Boards** package installed.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  by Matthew Heironimus - pull it via *Sketch > Include Library > Manage
  Libraries...* (query "Joystick"), or grab the source direct from the repo
  and side-load it with *Sketch > Include Library > Add .ZIP Library...*.

### Flash Protocol

1. Boot `EDJoystick/EDJoystick.ino` in the Arduino IDE.
2. Select **Tools > Board > Arduino Leonardo** (or Micro/Pro Micro, match
   your hardware).
3. Lock in the right **Tools > Port**.
4. Hit **Upload** and burn the firmware in.

Once it's flashed, the OS clocks the board as a generic USB joystick on
sight - no driver install, no handshake ritual. From there, bind it like any
other axis/button in your game or stick-mapping software. It's not chained
to Elite Dangerous or Star Citizen - it runs wherever the signal's read.

## Tuning the Rig

Every parameter you're likely to touch lives in the `USER CONFIGURATION`
block at the top of `EDJoystick.ino` - pins, axis inversion, response curve,
deadzone/recalibration behavior, and the debug feed toggle are all sitting
there as clean, commented constants. Key ones to know:

- **`VIEW_DATA`** - flip to `true`, reflash, and jack into a live Serial feed
  (115200 baud) showing raw/processed axis data, deadzone size, button
  state, and every recalibration event as it happens. Run it to verify your
  tuning before you commit. Kill it back to `false` for live runs.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - control how often, and how patiently, the
  rig re-centers itself when it goes dormant.

No reflash needed for day-to-day runs - calibration and deadzone tuning
happen autonomously on boot, and keep quietly self-correcting in the
background from there.

## Credits / Data Trail

- Built on the [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  by Matthew Heironimus.
- Root source / original schematic: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), sparked by a comment from Goustoulos.

## License

GNU General Public License v3.0 - full text in [LICENSE](LICENSE). Copy it,
fork it, wire it into whatever rig you're running.
