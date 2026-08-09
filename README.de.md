Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Arduino USB Daumen-Joystick - Zusätzlicher Schubcontroller

Ein kleiner analoger Daumen-Joystick, angeschlossen an einen Arduino Leonardo/Micro,
der sich dem PC gegenüber als gewöhnlicher USB-Joystick mit zwei Achsen und einem
Taster meldet.

Ursprünglich wurde er für **Elite Dangerous** gebaut, um den zwei zusätzlichen Achsen
das zu geben, was ein HAT-Schalter nicht kann: echten analogen vertikalen und
lateralen Schub, statt nur 4 Ein/Aus-Richtungen. Inzwischen ist er mit mir zu
**Star Citizen** umgezogen, und da er sich einfach als generischer USB-Joystick
anmeldet, ist er an keines der beiden Spiele gebunden - jede Software, die eine
Joystick-Achse akzeptiert, kann ihn nutzen. An einem Flightstick, der bereits
Roll/Nick/Gier (Roll/Pitch/Yaw) abdeckt, ergänzt dies ein echtes analoges
"Strafing" nach oben/unten und links/rechts.

## Funktionen

- Zwei analoge Achsen + ein Taster, gemeldet als Standard-USB-HID-Joystick.
- Selbstkalibrierender Mittelpunkt beim Start, mit konfigurierbarer Achsumkehr
  pro Achse.
- Periodische Neukalibrierung im Ruhezustand des Sticks, um Drift auszugleichen
  (Temperatur, Luftfeuchtigkeit, ein billiges Potentiometer, das sich einspielt) -
  ganz ohne Neustart. Die Neukalibrierung erfolgt ausschließlich, wenn der Stick
  sowohl unbewegt *als auch* nahe der Mitte liegt, niemals während der Benutzung.
- Selbstjustierende Totzone: Sie wird anhand des tatsächlich beobachteten Rauschens
  des Sticks bemessen statt anhand eines festen Schätzwerts, und verkleinert sich
  allmählich, je sicherer das System wird - so braucht ein loser/billiger Stick
  keine dauerhaft große Totzone.
- Optionale zweistufige Präzisionskurve und Einrasten auf Vollausschlag am Rand,
  beide standardmäßig deaktiviert (lineare Antwort).
- Status-LED-Rückmeldung und ein optionales Live-Debug-Protokoll über die
  serielle Schnittstelle zum Feinabstimmen.
- All das sind einfache Konstanten am Anfang des Sketches - keine Notwendigkeit,
  sich durch die Logik zu wühlen, um es neu einzustellen.

## Hardware

- Ein Arduino-Board mit nativer USB-HID-Unterstützung - **Leonardo**, **Micro**,
  oder **Pro Micro** (ATmega32u4). Boards ohne natives USB (z. B. Uno, Nano)
  können dies nicht ausführen.
- Ein analoges 2-Achsen-Joystick-Modul (zwei Potentiometer + ein Taster).

### Verkabelung

| Joystick-Pin | Arduino-Pin | Hinweise |
|---|---|---|
| VCC | 5V oder 3.3V | Siehe `VOLTAGE_SCALE` unten - muss zur tatsächlichen Verkabelung passen |
| GND | GND | |
| X-Achse (VRx) | A0 | `PIN_AXIS_X` |
| Y-Achse (VRy) | A1 | `PIN_AXIS_Y` |
| Taster (SW) | 6 | `PIN_BUTTON` - nutzt den internen Pull-up, kein externer Widerstand nötig |

Wird das Joystick-Modul mit 3,3V statt 5V versorgt, deckt sein analoger Ausgang
nur einen Teil des ADC-Bereichs ab - `VOLTAGE_SCALE` im Sketch gleicht das aus
(`1.0` für 5V, `1.5151` für 3.3V). Ist dieser Wert falsch, liegen die Achsen
dauerhaft nahe an einem Anschlag.

## Software

- [Arduino IDE](https://www.arduino.cc/en/software) (oder `arduino-cli`), mit
  installiertem Paket **Arduino AVR Boards**.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  von Matthew Heironimus - installierbar über *Sketch > Include Library > Manage
  Libraries...* (Suche nach "Joystick"), oder aus dem obigen Repository
  herunterladen und über *Sketch > Include Library > Add .ZIP Library...*
  hinzufügen.

### Kompilieren & Flashen

1. `EDJoystick/EDJoystick.ino` in der Arduino IDE öffnen.
2. **Tools > Board > Arduino Leonardo** auswählen (oder Micro/Pro Micro,
   passend zum eigenen Board).
3. Den richtigen **Tools > Port** auswählen.
4. Auf **Upload** klicken.

Nach dem Flashen sollte sich das Board dem Betriebssystem gegenüber als
generischer USB-Joystick melden - keine Treiber nötig. Von dort aus lässt es
sich wie jede andere Achse/Taste in eurem Spiel oder eurer
Joystick-Verwaltungssoftware belegen; es ist weder an Elite Dangerous noch an
Star Citizen gebunden.

## Konfiguration

Alles, was man wahrscheinlich ändern möchte, befindet sich im Block
`USER CONFIGURATION` am Anfang von `EDJoystick.ino` - Pins, Achsumkehr,
Antwortkurve, Totzonen-/Neukalibrierungs-Abstimmung und der Debug-Schalter sind
dort alles einfache, kommentierte Konstanten. Insbesondere:

- **`VIEW_DATA`** - auf `true` setzen und neu flashen, um über die serielle
  Schnittstelle (115200 Baud) eine Live-Anzeige der rohen/verarbeiteten
  Achswerte, der Totzone, des Tasterzustands und der Kalibrierungsereignisse
  zu erhalten. Nützlich, um Abstimmungsänderungen zu bestätigen, bevor man sie
  festlegt. Für den normalen Betrieb auf `false` belassen.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - steuern, wie oft und wie geduldig sich der
  Controller im Ruhezustand selbst neu zentriert.

Für den täglichen Gebrauch ist kein erneutes Flashen nötig - Kalibrierung und
Totzonen-Abstimmung erfolgen automatisch beim Start und danach periodisch.

## Danksagung

- Basiert auf der [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  von Matthew Heironimus.
- Ursprüngliche Quelle / ursprünglicher Ansatz: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), basierend auf einem Kommentar von Goustoulos.

## Lizenz

GNU General Public License v3.0 - siehe [LICENSE](LICENSE).
