Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Arduino USB Thumb Joystick - Auxiliary Thrust Controller

A small analog thumb joystick, wired to an Arduino Leonardo/Micro, that shows up to your
PC as a plain USB joystick with two axes and one button.

It was originally built for **Elite Dangerous**, to give the two extra axes a HAT switch
can't: real analog vertical and lateral thrust, instead of 4 on/off directions. It's
since moved with me to **Star Citizen**, and because it just enumerates as a generic USB
joystick, it isn't tied to either game - anything that accepts a joystick axis can use it.
On a flight stick that already covers roll/pitch/yaw, this adds proper analog "strafe"
up/down and left/right.

## Features

- Two analog axes + one button, reported as a standard USB HID joystick.
- Self-calibrating center point at boot, with configurable per-axis invert.
- Periodic recalibration while the stick is idle, to correct for drift
  (temperature, humidity, a cheap pot settling in) without needing a reboot -
  it only ever recalibrates when the stick is both unmoved *and* resting near
  center, never mid-use.
- Self-tuning deadzone: sized to the stick's actual observed jitter instead of
  a fixed guess, shrinking gradually as it gets more confident, so a
  loose/cheap stick doesn't need a large permanent deadzone.
- Optional dual-slope precision response curve and edge snap-to-full, both
  off by default (linear response).
- Status LED feedback and an optional live Serial debug readout for tuning.
- All of the above are simple constants at the top of the sketch - no need to
  dig through the logic to retune it.

## Hardware

- An Arduino board with native USB HID support - **Leonardo**, **Micro**, or
  **Pro Micro** (ATmega32u4). Boards without native USB (e.g. Uno, Nano)
  cannot run this.
- A 2-axis analog joystick module (two potentiometers + a click button).

### Wiring

| Joystick Pin | Arduino Pin | Notes |
|---|---|---|
| VCC | 5V or 3.3V | See `VOLTAGE_SCALE` below - must match how it's wired |
| GND | GND | |
| X axis (VRx) | A0 | `PIN_AXIS_X` |
| Y axis (VRy) | A1 | `PIN_AXIS_Y` |
| Button (SW) | 6 | `PIN_BUTTON` - uses the internal pull-up, no external resistor needed |

If the joystick module is powered from 3.3V instead of 5V, its analog output
only swings across part of the ADC's range - `VOLTAGE_SCALE` in the sketch
corrects for that (`1.0` for 5V, `1.5151` for 3.3V). Get this wrong and the
axes will read as permanently near one edge.

## Software

- [Arduino IDE](https://www.arduino.cc/en/software) (or `arduino-cli`), with
  the **Arduino AVR Boards** package installed.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  by Matthew Heironimus - install it via *Sketch > Include Library > Manage
  Libraries...* (search "Joystick"), or download it from the repo above and
  add it via *Sketch > Include Library > Add .ZIP Library...*.

### Building & Flashing

1. Open `EDJoystick/EDJoystick.ino` in the Arduino IDE.
2. Select **Tools > Board > Arduino Leonardo** (or Micro/Pro Micro, matching
   your board).
3. Select the correct **Tools > Port**.
4. Click **Upload**.

Once flashed, the board should show up to the OS as a generic USB joystick -
no drivers needed. From there it can be bound like any other axis/button in
your game or flight-stick management software; it isn't specific to Elite
Dangerous or Star Citizen.

## Configuration

Everything you're likely to want to change lives in the `USER CONFIGURATION`
block at the top of `EDJoystick.ino` - pins, axis inversion, response curve,
deadzone/recalibration tuning, and the debug output toggle are all plain,
commented constants there. In particular:

- **`VIEW_DATA`** - set to `true` and reflash to get a live readout over
  Serial (115200 baud) of raw/processed axis values, deadzone, button state,
  and calibration events. Useful for confirming tuning changes before
  committing to them. Leave it `false` for normal use.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - control how often and how patiently the
  controller re-centers itself while idle.

No re-flash is needed to *use* the controller day-to-day - calibration and
deadzone tuning happen automatically at boot and periodically thereafter.

## Credits

- Built on the [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  by Matthew Heironimus.
- Root source / original approach: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), based on a comment by Goustoulos.

## License

GNU General Public License v3.0 - see [LICENSE](LICENSE).
