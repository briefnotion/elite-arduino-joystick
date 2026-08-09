Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Joystick USB de Pulgar para Arduino - Controlador de Empuje Auxiliar

Un pequeño joystick analógico de pulgar, conectado a un Arduino Leonardo/Micro, que se
presenta ante tu PC como un joystick USB normal con dos ejes y un botón.

Originalmente se construyó para **Elite Dangerous**, para darle a los dos ejes extra algo
que un HAT (sombrero direccional) no puede ofrecer: empuje analógico real, vertical y
lateral, en lugar de 4 direcciones de encendido/apagado. Desde entonces se ha mudado
conmigo a **Star Citizen**, y como simplemente se presenta como un joystick USB genérico,
no está atado a ningún juego en particular - cualquier programa que acepte un eje de
joystick puede usarlo. En un stick de vuelo que ya cubre alabeo/cabeceo/guiñada
(roll/pitch/yaw), esto añade un auténtico "desplazamiento lateral" (strafe) analógico,
arriba/abajo e izquierda/derecha.

## Características

- Dos ejes analógicos + un botón, reportados como un joystick USB HID estándar.
- Punto central autocalibrado al arrancar, con inversión configurable por eje.
- Recalibración periódica mientras el stick está en reposo, para corregir la deriva
  (temperatura, humedad, un potenciómetro barato asentándose) sin necesidad de reiniciar -
  solo recalibra cuando el stick está a la vez sin movimiento *y* cerca del centro,
  nunca mientras se está usando.
- Zona muerta autoajustable: dimensionada según el ruido/vibración realmente observado
  del stick en lugar de un valor fijo estimado, reduciéndose gradualmente conforme gana
  confianza, para que un stick barato o suelto no necesite una zona muerta permanentemente
  grande.
- Curva de respuesta de precisión de doble pendiente y ajuste a fondo de escala en los
  bordes, ambos opcionales y desactivados por defecto (respuesta lineal).
- Retroalimentación mediante el LED de estado y una lectura de depuración opcional en
  vivo por puerto Serial para el ajuste fino.
- Todo lo anterior son simples constantes al principio del sketch - no hace falta
  rebuscar en la lógica para reajustarlo.

## Hardware

- Una placa Arduino con soporte USB HID nativo - **Leonardo**, **Micro**, o
  **Pro Micro** (ATmega32u4). Las placas sin USB nativo (p. ej. Uno, Nano)
  no pueden ejecutar esto.
- Un módulo de joystick analógico de 2 ejes (dos potenciómetros + un botón pulsador).

### Cableado

| Pin del Joystick | Pin de Arduino | Notas |
|---|---|---|
| VCC | 5V o 3.3V | Ver `VOLTAGE_SCALE` más abajo - debe coincidir con cómo está cableado |
| GND | GND | |
| Eje X (VRx) | A0 | `PIN_AXIS_X` |
| Eje Y (VRy) | A1 | `PIN_AXIS_Y` |
| Botón (SW) | 6 | `PIN_BUTTON` - usa la resistencia pull-up interna, no necesita resistencia externa |

Si el módulo del joystick se alimenta con 3.3V en lugar de 5V, su salida analógica solo
recorre una parte del rango del ADC - `VOLTAGE_SCALE` en el sketch corrige esto (`1.0`
para 5V, `1.5151` para 3.3V). Si esto se configura mal, los ejes se leerán permanentemente
cerca de un extremo.

## Software

- [Arduino IDE](https://www.arduino.cc/en/software) (o `arduino-cli`), con el
  paquete **Arduino AVR Boards** instalado.
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  de Matthew Heironimus - instálala desde *Sketch > Include Library > Manage
  Libraries...* (busca "Joystick"), o descárgala del repositorio anterior y
  añádela mediante *Sketch > Include Library > Add .ZIP Library...*.

### Compilación y Carga

1. Abre `EDJoystick/EDJoystick.ino` en el IDE de Arduino.
2. Selecciona **Tools > Board > Arduino Leonardo** (o Micro/Pro Micro, según
   tu placa).
3. Selecciona el **Tools > Port** correcto.
4. Haz clic en **Upload**.

Una vez cargado, la placa debería aparecer ante el sistema operativo como un joystick
USB genérico - sin necesidad de drivers. A partir de ahí se puede asignar como cualquier
otro eje/botón en tu juego o software de gestión de mandos; no es específico de Elite
Dangerous ni de Star Citizen.

## Configuración

Todo lo que probablemente quieras cambiar está en el bloque `USER CONFIGURATION`
al principio de `EDJoystick.ino` - pines, inversión de ejes, curva de respuesta,
ajuste de zona muerta/recalibración, y el interruptor de depuración son todas
constantes sencillas y comentadas ahí. En particular:

- **`VIEW_DATA`** - ponlo en `true` y vuelve a cargar el sketch para obtener una
  lectura en vivo por puerto Serial (115200 baudios) de los valores de los ejes
  crudos/procesados, la zona muerta, el estado del botón y los eventos de
  calibración. Útil para confirmar los cambios de ajuste antes de dejarlos fijos.
  Déjalo en `false` para el uso normal.
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - controlan con qué frecuencia y con cuánta
  paciencia el controlador se recentra a sí mismo mientras está en reposo.

No hace falta volver a cargar el sketch para el uso diario del controlador - la
calibración y el ajuste de la zona muerta ocurren automáticamente al arrancar y
periódicamente después.

## Créditos

- Construido sobre [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  de Matthew Heironimus.
- Fuente original / enfoque de partida: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables), basado en un comentario de Goustoulos.

## Licencia

GNU General Public License v3.0 - ver [LICENSE](LICENSE).
