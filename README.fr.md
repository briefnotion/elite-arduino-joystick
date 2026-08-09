Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Mini-Joystick USB Arduino - Contrôleur de Poussée Auxiliaire

Un petit joystick analogique, câblé sur un Arduino Leonardo/Micro, qui se présente à
votre PC comme un simple joystick USB avec deux axes et un bouton.

Il a été conçu à l'origine pour **Elite Dangerous**, afin de donner aux deux axes
supplémentaires ce qu'un HAT (chapeau directionnel) ne peut pas offrir : une véritable
poussée analogique verticale et latérale, au lieu de 4 directions tout ou rien. Depuis,
il m'a suivi vers **Star Citizen**, et comme il se présente simplement comme un joystick
USB générique, il n'est lié à aucun des deux jeux - tout logiciel acceptant un axe de
joystick peut l'utiliser. Sur un manche qui couvre déjà le roulis/tangage/lacet
(roll/pitch/yaw), cela ajoute un véritable déplacement latéral ("strafe") analogique,
haut/bas et gauche/droite.

## Fonctionnalités

- Deux axes analogiques + un bouton, reportés comme un joystick USB HID standard.
- Point central auto-calibré au démarrage, avec inversion configurable par axe.
- Recalibration périodique lorsque le manche est immobile, pour corriger la dérive
  (température, humidité, un potentiomètre bon marché qui se stabilise) sans avoir
  besoin de redémarrer - la recalibration ne se déclenche que lorsque le manche est
  à la fois immobile *et* proche du centre, jamais en cours d'utilisation.
- Zone morte auto-ajustable : dimensionnée d'après le bruit réellement observé du
  manche plutôt qu'une estimation fixe, se réduisant progressivement à mesure que
  la confiance augmente, afin qu'un manche bon marché ou imprécis n'ait pas besoin
  d'une grande zone morte permanente.
- Courbe de réponse de précision à double pente et accrochage en fin de course,
  tous deux optionnels et désactivés par défaut (réponse linéaire).
- Retour visuel par LED d'état et lecture de débogage en direct optionnelle via
  le port Série pour le réglage.
- Tout ce qui précède se résume à de simples constantes en haut du sketch - pas
  besoin de fouiller dans la logique pour le réajuster.

## Matériel

- Une carte Arduino avec support USB HID natif - **Leonardo**, **Micro**, ou
  **Pro Micro** (ATmega32u4). Les cartes sans USB natif (par ex. Uno, Nano)
  ne peuvent pas faire fonctionner ce programme.
- Un module joystick analogique 2 axes (deux potentiomètres + un bouton poussoir).

### Câblage

| Broche du Joystick | Broche Arduino | Notes |
|---|---|---|
| VCC | 5V ou 3.3V | Voir `VOLTAGE_SCALE` ci-dessous - doit correspondre au câblage réel |
| GND | GND | |
| Axe X (VRx) | A0 | `PIN_AXIS_X` |
| Axe Y (VRy) | A1 | `PIN_AXIS_Y` |
| Bouton (SW) | 6 | `PIN_BUTTON` - utilise la résistance de tirage interne, pas de résistance externe nécessaire |

Si le module joystick est alimenté en 3.3V plutôt qu'en 5V, sa sortie analogique ne
balaie qu'une partie de la plage du convertisseur analogique-numérique - `VOLTAGE_SCALE`
dans le sketch corrige cela (`1.0` pour 5V, `1.5151` pour 3.3V). Une mauvaise valeur ici
fera lire les axes en permanence près d'un bord.

## Logiciel

- [Arduino IDE](https://www.arduino.cc/en/software) (ou `arduino-cli`), avec le
  paquet **Arduino AVR Boards** installé.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  de Matthew Heironimus - installez-la via *Sketch > Include Library > Manage
  Libraries...* (recherchez "Joystick"), ou téléchargez-la depuis le dépôt
  ci-dessus et ajoutez-la via *Sketch > Include Library > Add .ZIP Library...*.

### Compilation et Téléversement

1. Ouvrez `EDJoystick/EDJoystick.ino` dans l'IDE Arduino.
2. Sélectionnez **Tools > Board > Arduino Leonardo** (ou Micro/Pro Micro,
   selon votre carte).
3. Sélectionnez le bon **Tools > Port**.
4. Cliquez sur **Upload**.

Une fois téléversé, la carte devrait apparaître pour le système d'exploitation comme
un joystick USB générique - aucun pilote nécessaire. À partir de là, elle peut être
assignée comme n'importe quel autre axe/bouton dans votre jeu ou logiciel de gestion
de manches ; elle n'est spécifique ni à Elite Dangerous ni à Star Citizen.

## Configuration

Tout ce que vous voudrez probablement modifier se trouve dans le bloc
`USER CONFIGURATION` en haut de `EDJoystick.ino` - broches, inversion des axes,
courbe de réponse, réglage de la zone morte/recalibration, et l'interrupteur de
débogage sont toutes de simples constantes commentées à cet endroit. En particulier :

- **`VIEW_DATA`** - passez-le à `true` et reflashez pour obtenir une lecture en
  direct via le port Série (115200 bauds) des valeurs d'axes brutes/traitées, de
  la zone morte, de l'état du bouton et des événements de calibration. Utile pour
  vérifier les réglages avant de les valider. Laissez à `false` pour une utilisation
  normale.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - contrôlent la fréquence et la patience avec
  lesquelles le contrôleur se recentre lui-même en cas d'inactivité.

Aucun nouveau flashage n'est nécessaire pour l'utilisation quotidienne du
contrôleur - la calibration et le réglage de la zone morte se font automatiquement
au démarrage puis périodiquement par la suite.

## Crédits

- Construit sur la [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  de Matthew Heironimus.
- Source d'origine / approche initiale : [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), basé sur un commentaire de Goustoulos.

## Licence

GNU General Public License v3.0 - voir [LICENSE](LICENSE).
