Language: [English](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.md), [Español](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.es.md), [Français](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.fr.md), [Deutsch](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.de.md), [日本語](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.ja.md), [中文](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.zh.md), [Corporate](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.corporate.md), [Cajun](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cajun.md), [CyberPunk](https://github.com/briefnotion/elite-arduino-joystick/blob/master/README.cyberpunk.md)

# Arduino USB 親指ジョイスティック - 補助推力コントローラー

Arduino Leonardo/Micro に配線された小さなアナログ親指ジョイスティックで、PC からは
2軸1ボタンの普通の USB ジョイスティックとして認識されます。

もともとは **Elite Dangerous** 用に作られたもので、HAT スイッチにはできないこと -
オン/オフの4方向ではなく、本物のアナログな上下・左右のスラスト - を、この2つの
追加軸に持たせるためのものでした。その後、私と一緒に **Star Citizen** へも
移りましたが、単なる汎用 USB ジョイスティックとして認識されるだけなので、
どちらのゲームにも縛られません - ジョイスティック軸を受け付けるソフトウェアなら
何でも使えます。ロール・ピッチ・ヨーをすでにカバーしているフライトスティックに、
上下・左右の本格的なアナログ「ストレイフ」を追加します。

## 特徴

- 2つのアナログ軸 + 1つのボタンを、標準的な USB HID ジョイスティックとして送信。
- 起動時に自動でセンター位置をキャリブレーション、軸ごとに反転設定も可能。
- スティックが静止している間の定期的な再キャリブレーションにより、ドリフト
  (温度、湿度、安物ポテンショメータの経時変化) を再起動なしで補正 - スティックが
  動いておらず、かつセンター付近にある場合のみ再キャリブレーションが行われ、
  操作中に発生することはありません。
- 自己調整デッドゾーン: 固定の推定値ではなく、実際に観測されたスティックの
  ジッターに合わせてサイズが決まり、確信が強まるにつれて徐々に縮小するため、
  ガタつきのある安物スティックでも常時大きなデッドゾーンを必要としません。
- オプションの2段階プレシジョンカーブと端でのフルスケールへのスナップ機能
  (どちらもデフォルトでは無効、線形応答)。
- ステータス LED によるフィードバックと、チューニング用のオプションのライブ
  シリアルデバッグ出力。
- 上記はすべてスケッチの先頭にあるシンプルな定数です - 再調整のためにロジックを
  掘り下げる必要はありません。

## ハードウェア

- ネイティブ USB HID をサポートする Arduino ボード - **Leonardo**、**Micro**、
  または **Pro Micro** (ATmega32u4)。ネイティブ USB を持たないボード
  (Uno、Nano など) では動作しません。
- 2軸アナログジョイスティックモジュール (2つのポテンショメータ + クリックボタン)。

### 配線

| ジョイスティック側ピン | Arduino 側ピン | 備考 |
|---|---|---|
| VCC | 5V または 3.3V | 下記の `VOLTAGE_SCALE` を参照 - 実際の配線と一致させること |
| GND | GND | |
| X軸 (VRx) | A0 | `PIN_AXIS_X` |
| Y軸 (VRy) | A1 | `PIN_AXIS_Y` |
| ボタン (SW) | 6 | `PIN_BUTTON` - 内部プルアップを使用、外部抵抗は不要 |

ジョイスティックモジュールが 5V ではなく 3.3V で駆動されている場合、アナログ出力は
ADC の範囲の一部しか使いません - スケッチ内の `VOLTAGE_SCALE` がこれを補正します
(5V なら `1.0`、3.3V なら `1.5151`)。この値を間違えると、軸が常にどちらかの端付近の
値を示すようになります。

## ソフトウェア

- [Arduino IDE](https://www.arduino.cc/en/software) (または `arduino-cli`)、
  **Arduino AVR Boards** パッケージがインストールされていること。
- Matthew Heironimus 氏による
  [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary) -
  *Sketch > Include Library > Manage Libraries...* から ("Joystick" で検索)
  インストールするか、上記リポジトリからダウンロードして
  *Sketch > Include Library > Add .ZIP Library...* で追加してください。

### ビルドと書き込み

1. Arduino IDE で `EDJoystick/EDJoystick.ino` を開く。
2. **Tools > Board > Arduino Leonardo** を選択 (使用しているボードに応じて
   Micro/Pro Micro でも可)。
3. 正しい **Tools > Port** を選択する。
4. **Upload** をクリックする。

書き込みが完了すると、ボードは OS に対して汎用 USB ジョイスティックとして
認識されるはずです - ドライバは不要です。あとはお使いのゲームやスティック管理
ソフトウェアで、他の軸やボタンと同様に割り当てることができます。Elite Dangerous
や Star Citizen 専用というわけではありません。

## 設定

変更したくなりそうな項目はすべて `EDJoystick.ino` の先頭にある
`USER CONFIGURATION` ブロックにまとまっています - ピン設定、軸の反転、応答カーブ、
デッドゾーン/再キャリブレーションの調整、デバッグ出力の切り替えは、すべてそこに
コメント付きの単純な定数として置かれています。特に:

- **`VIEW_DATA`** - `true` に設定して再書き込みすると、シリアル (115200 baud)
  経由で生/処理済みの軸の値、デッドゾーン、ボタンの状態、キャリブレーション
  イベントをライブで確認できます。設定変更を確定する前の確認に便利です。
  通常使用時は `false` のままにしてください。
- **`ENABLE_PERIODIC_RECALIBRATION`** / `RECALIBRATION_IDLE_MS` /
  `RECALIBRATION_INTERVAL_MS` - スティックが静止している間、どのくらいの頻度と
  忍耐強さでコントローラーが自身を再センタリングするかを制御します。

日常的にコントローラーを *使う* だけなら再書き込みは不要です - キャリブレーションと
デッドゾーンの調整は、起動時および以降定期的に自動で行われます。

## クレジット

- Matthew Heironimus 氏による
  [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  を利用しています。
- 元ネタ / 元のアプローチ: [Arduino Leonardo/Micro as Game Controller/Joystick](https://www.instructables.com/id/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
  (Instructables)、Goustoulos 氏のコメントを基にしています。

## ライセンス

GNU General Public License v3.0 - [LICENSE](LICENSE) を参照してください。
