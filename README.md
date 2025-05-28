# ESP32 Bluetooth Signal Jammer

Build a Bluetooth signal jamming device using the Arduino code and schematics here.

The default setup uses an ESP32 dev board for the MCU and 2x NRF24L01 radio transmitters for transmitting. It can easily be tweaked to interfere with anything that uses the 2.4GHz band (WiFi, RC toys, drones etc).

## ⚠️ LEGAL WARNING

This code repository is for educational purposes only! DO NOT use a signal jamming device as they can can interfere with public safety communications and emergency services. In many countries the use of signal jammers is prohibited by law and intentional interference with wireless signals can result in severe penalties.

## Parts List

Build your device on a breadboard or perfboard. All the parts needed are listed below with a link to buy them on AliExpress.

- [ESP32 Dev Board](https://s.click.aliexpress.com/e/_oBzks2E)
- [2x NRF24L01+PA+LNA](https://s.click.aliexpress.com/e/_okUsZpp)
- [2x Capacitor (10µF – 47µF)](https://s.click.aliexpress.com/e/_olkWSDz)
- [Breadboard](https://s.click.aliexpress.com/e/_on6KzoP)
- [Jumper Wires](https://s.click.aliexpress.com/e/_ooo7z5h)
- [Power Bank](https://s.click.aliexpress.com/e/_oneC3BV)

## Wiring Diagram

Follow the wiring below using the same pins and the code will work out of the box with no adjustments needed. Plug the antenna in before powering the NRF24L01 module! Otherwise the amplifier will be burned out.

<img src="https://github.com/stuthemoo/ESP32BluetoothJammer/raw/main/images/wiring-breadboard.png" width="1000" alt="Breadboard Wiring Diagram">

## Photos

### Working Breadboard Prototype

<img src="https://github.com/stuthemoo/ESP32BluetoothJammer/raw/main/images/prototype-1.jpg" width="500" alt="Breadboard Prototype 1">

<img src="https://github.com/stuthemoo/ESP32BluetoothJammer/raw/main/images/prototype-2.jpg" width="500" alt="Breadboard Prototype 2">

## Uploading

Upload this code to your ESP32. Recommended method using Arduino IDE:

1. Download the latest [Arduino IDE](https://www.arduino.cc/en/software).

2. Install ESP32 board support from Espressif Systems ([Full guide found here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)).

3. Install `RF24 by TMRh20` from the library manager.

4. Open the `BluetoothJammer/BluetoothJammer.imo` file, connect your ESP32, select the port/board and then click upload.

## Contributing

Pull requests are welcome. Please open an issue if you find any problems.

## License

[GNU General Public License v3.0](https://choosealicense.com/licenses/gpl-3.0/)