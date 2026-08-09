Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Arduino USB Thumb Joystick - Dat Extra Thrust Contraption, Cher

Awright, cher, gather 'round. This here's a li'l bitty analog thumb joystick,
wired up good to an Arduino Leonardo/Micro, and it shows up on your 'puter
just like any ol' plain USB joystick - two axes and one button, sittin'
right there ready to go. I garontee.

Now I built this thing way back for **Elite Dangerous**, 'cause dem HAT
switches, mais, they ain't nothin' but four li'l on/off directions actin'
like they runnin' the show. Didn't want that. Wanted real analog thrust,
up-down and side-to-side, smooth like a good roux. Since then this ol'
gadget done followed me on over to **Star Citizen** too, and don't need no
fixin' up to do it, no - 'cause far as your computer's concerned, it's just
a plain ol' USB joystick, not married to nary one game. Anything that'll
take a joystick axis, this'll work with it, yeah you rite. Slap it on a
flight stick that's already handlin' roll, pitch, and yaw, and now you got
yourself real strafe - up, down, left, right - like it oughta be.

## What She Does

- Two analog axes plus one button, comin' in like a proper USB HID joystick,
  no funny business.
- Finds her own center point when she boots up, and you can flip either
  axis around if it's backwards on ya.
- Every so often, while the stick's just sittin' there mindin' its own
  business, she'll quietly recalibrate herself to chase off the drift -
  heat, humidity, a cheap ol' pot settlin' in over time - no need to go
  rebootin' nothin'. Only does it when the stick ain't movin' *and* it's
  restin' near center, never while you playin', I garontee that.
- Deadzone tunes itself too - sizes up to whatever jitter she actually sees
   'stead of some number somebody guessed at, and she eases it down slow
  once she's sure, so a loose cheap stick don't need a big fat deadzone
  forever.
- Got a fancy two-slope precision curve and a snap-to-full-throw-at-the-edge
  trick, if you want 'em - both turned off by default, keepin' it straight
  and simple.
- Li'l status light lets you know what's goin' on, and if you want to watch
  her work, she'll talk to you over Serial too.
- All this here tunin' is just plain constants sittin' right at the top of
  the sketch - ain't gotta go diggin' through the whole thing to change it.

## What You Gonna Need

- An Arduino board with real native USB - **Leonardo**, **Micro**, or
  **Pro Micro** (ATmega32u4). Boards without it, like the Uno or Nano,
  ain't gonna cut it, no sir.
- A 2-axis analog joystick module - two pots and a click button.

### Wirin' It Up

| Joystick Pin | Arduino Pin | Notes |
|---|---|---|
| VCC | 5V or 3.3V | Check `VOLTAGE_SCALE` down below - gotta match how you wired it |
| GND | GND | |
| X axis (VRx) | A0 | `PIN_AXIS_X` |
| Y axis (VRy) | A1 | `PIN_AXIS_Y` |
| Button (SW) | 6 | `PIN_BUTTON` - uses the built-in pull-up, don't need no extra resistor |

Now if you wirin' that joystick module off 3.3V 'stead of 5V, her analog
signal only gonna cover part of what the ADC can see - `VOLTAGE_SCALE` in
the sketch fixes that right up (`1.0` for 5V, `1.5151` for 3.3V). Get that
wrong, cher, and your axes gonna sit stuck near one edge like they done give
up.

## What Software You Need

- [Arduino IDE](https://www.arduino.cc/en/software) (or `arduino-cli` if
  that's your style), with the **Arduino AVR Boards** package put in.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  by Matthew Heironimus - grab it through *Sketch > Include Library > Manage
  Libraries...* (look up "Joystick"), or download it yourself from that
  there repo and add it with *Sketch > Include Library > Add .ZIP Library...*.

### Buildin' & Flashin' Her Up

1. Pop open `EDJoystick/EDJoystick.ino` in the Arduino IDE.
2. Go on and pick **Tools > Board > Arduino Leonardo** (or Micro/Pro Micro,
   whatever you got).
3. Pick the right **Tools > Port**.
4. Hit **Upload** and let her cook.

Once she's flashed, your computer oughta see her as a plain ol' USB joystick
- no drivers to go huntin' down. From there just bind her axes and button
like anything else in your game or your stick-manager software; she don't
care if it's Elite Dangerous, Star Citizen, or nothin' else, no.

## Tunin' Her Up

Everything you likely gonna wanna mess with is sittin' right up top in the
`USER CONFIGURATION` block of `EDJoystick.ino` - pins, flippin' the axes,
the response curve, deadzone and recalibration settin's, and the debug
switch, all plain lil' constants with notes right there. 'Specially these:

- **`VIEW_DATA`** - flip her to `true` and reflash, and you'll get a live
  readout over Serial (115200 baud) showin' raw and processed axis numbers,
  the deadzone, the button, and when she recalibrates. Good for checkin'
  your work 'fore you settle on it. Leave her `false` for regular playin'.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - decide how often, and how patient she is,
   'bout recenterin' herself while she's restin'.

Don't need to reflash nothin' for regular day-to-day usin' - she calibrates
and tunes that deadzone all on her own, at startup and every so often after
that, no fuss.

## Much Obliged To

- Built on top of the [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  by Matthew Heironimus.
- Where this whole thing got its start: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), off a comment by Goustoulos.

## License

GNU General Public License v3.0 - see [LICENSE](LICENSE). Laissez les bons
temps rouler.
