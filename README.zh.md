Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Arduino USB 拇指摇杆 - 辅助推力控制器

一个连接到 Arduino Leonardo/Micro 的小型模拟拇指摇杆,在电脑上会被识别为一个普通的
USB 摇杆,拥有两个轴和一个按钮。

它最初是为 **Elite Dangerous** 制作的,目的是让这两个额外的轴实现 HAT 帽子开关做不到
的事情:真正的模拟垂直和横向推力,而不是 4 个开/关方向。后来它跟着我一起转到了
**Star Citizen**,而且因为它在系统里只是作为一个通用 USB 摇杆出现,所以并不局限于
这两款游戏 - 任何接受摇杆轴输入的软件都可以使用它。在一个已经覆盖了滚转/俯仰/偏航
(roll/pitch/yaw) 的飞行摇杆上,它增加了真正的模拟"平移" (strafe) 上下和左右控制。

## 功能特性

- 两个模拟轴 + 一个按钮,以标准 USB HID 摇杆的形式上报。
- 启动时自动校准中心点,并可按轴单独配置反转。
- 摇杆静止时会周期性地重新校准,以修正漂移(温度、湿度、廉价电位器的老化沉降)
  而无需重启 - 只有当摇杆既没有移动、又停留在中心附近时才会重新校准,绝不会在
  使用过程中进行。
- 自调整死区:根据摇杆实际观测到的抖动来确定大小,而不是使用固定的估计值,并
  随着置信度提高而逐渐收窄,这样一个松旷/廉价的摇杆就不需要永久保持一个很大的
  死区。
- 可选的双斜率精度响应曲线,以及边缘满量程吸附功能,两者默认均为关闭(线性响应)。
  。
- 状态指示灯反馈,以及可选的实时串口调试输出,便于调校。
- 以上所有选项都只是脚本开头的简单常量 - 无需深入代码逻辑即可重新调整。

## 硬件要求

- 一块支持原生 USB HID 的 Arduino 开发板 - **Leonardo**、**Micro**,或
  **Pro Micro** (ATmega32u4)。不支持原生 USB 的开发板(例如 Uno、Nano)
  无法运行此程序。
- 一个双轴模拟摇杆模块(两个电位器 + 一个按压按钮)。

### 接线

| 摇杆引脚 | Arduino 引脚 | 备注 |
|---|---|---|
| VCC | 5V 或 3.3V | 参见下方的 `VOLTAGE_SCALE` - 必须与实际接线方式一致 |
| GND | GND | |
| X 轴 (VRx) | A0 | `PIN_AXIS_X` |
| Y 轴 (VRy) | A1 | `PIN_AXIS_Y` |
| 按钮 (SW) | 6 | `PIN_BUTTON` - 使用内部上拉电阻,无需外接电阻 |

如果摇杆模块使用 3.3V 而不是 5V 供电,其模拟输出只会覆盖 ADC 量程的一部分 -
脚本中的 `VOLTAGE_SCALE` 会对此进行校正(5V 时为 `1.0`,3.3V 时为 `1.5151`)。
如果这个值设置错误,轴的读数会一直停留在接近某一端的位置。

## 软件要求

- [Arduino IDE](https://www.arduino.cc/en/software)(或 `arduino-cli`),
  并安装 **Arduino AVR Boards** 开发板包。
- 由 Matthew Heironimus 开发的
  [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary) -
  可通过 *Sketch > Include Library > Manage Libraries...*(搜索 "Joystick")
  安装,或从上述仓库下载后通过 *Sketch > Include Library > Add .ZIP Library...*
  添加。

### 编译与烧录

1. 在 Arduino IDE 中打开 `EDJoystick/EDJoystick.ino`。
2. 选择 **Tools > Board > Arduino Leonardo**(或根据你的开发板选择
   Micro/Pro Micro)。
3. 选择正确的 **Tools > Port**。
4. 点击 **Upload**。

烧录完成后,开发板应会被操作系统识别为一个通用 USB 摇杆 - 无需驱动程序。之后就
可以像绑定其他任何轴/按钮一样,在你的游戏或摇杆管理软件中进行绑定;它并不局限于
Elite Dangerous 或 Star Citizen。

## 配置说明

你可能想要修改的所有内容都位于 `EDJoystick.ino` 开头的 `USER CONFIGURATION` 代码块中
- 引脚、轴反转、响应曲线、死区/重新校准调节,以及调试输出开关,都是那里带注释的
简单常量。特别需要注意:

- **`VIEW_DATA`** - 设为 `true` 并重新烧录后,可通过串口(115200 波特率)实时查看
  原始/处理后的轴数值、死区大小、按钮状态以及校准事件。这在确认调校效果、正式
  应用之前很有用。日常使用时请保持为 `false`。
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - 控制控制器在静止时重新校准自身中心的频率和
  耐心程度。

日常*使用*控制器时无需重新烧录 - 校准和死区调整会在启动时以及之后周期性地自动
进行。

## 鸣谢

- 基于 Matthew Heironimus 开发的
  [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  构建。
- 原始来源 / 最初的思路:[Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables),基于 Goustoulos 的一条评论。

## 许可证

GNU General Public License v3.0 - 详见 [LICENSE](LICENSE)。
