# ESP32 Bluetooth Signal Jammer

Build a Bluetooth signal jamming device using the Arduino code and schematics here.

This setup uses an ESP32 dev board for the MCU and 2x NRF24L01 radio transmitters for transmitting. It can easily be tweaked to interfere with anything that uses the 2.4GHz band (WiFi, RC toys, drones etc).

## ⚠️ LEGAL WARNING

This code repository is for educational purposes only! DO NOT use a signal jamming device as they can can interfere with public safety communications and emergency services. In many countries the use of signal jammers is prohibited by law and intentional interference with wireless signals can result in severe penalties.

## Parts List

Build your device on a breadboard or perfboard.

All the parts needed are listed below with links to buy them from AliExpress or Amazon.

#### Parts on AliExpress

- [ESP32 Dev Board](https://s.click.aliexpress.com/e/_oBzks2E)
- [2x NRF24L01+PA+LNA](https://s.click.aliexpress.com/e/_okUsZpp)
- [Breadboard](https://s.click.aliexpress.com/e/_on6KzoP)
- [Jumper Wires](https://s.click.aliexpress.com/e/_ooo7z5h)
- [2x Capacitor (10µF – 47µF)](https://s.click.aliexpress.com/e/_olkWSDz)


- [Power Bank](https://s.click.aliexpress.com/e/_oneC3BV) **or** [3.7v Battery & Charger](https://s.click.aliexpress.com/e/_oElYgVl)

#### Parts on Amazon (affiliate links)

- [ESP32 Dev Board](https://amzn.to/4j8IFjW)
- [2x NRF24L01+PA+LNA](https://amzn.to/43nGRhg)
- [Breadboard](https://amzn.to/4k4pFEF)
- [Jumper Wires](https://amzn.to/45fErUf)
- [2x Capacitor (10µF – 47µF)](https://amzn.to/4kvqo1v)


- [Power Bank](https://amzn.to/43eFnqV) **or** [3.7v Battery](https://amzn.to/3SHqebj) + [Charger](https://amzn.to/4k1NNrt)

## Wiring Diagrams

Follow the wiring below using the same pins and the code will work out of the box with no adjustments needed.

![Breadboard Wiring Diagram](https://github.com/stuthemoo/ESP32BluetoothJammer/raw/main/wiring-breadboard.png "Breadboard Wiring Diagram")

## Uploading

Follow the wiring diagram and upload this code to your device use the Arduino IDE with ESP32 support and RF24 library installed.

1. [Download the Arduino IDE](https://www.arduino.cc/en/software).

2. Install Arduino-ESP32 support. [Full guide found here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

3. Install `RF24 (by TMRh20)` from library manager.

4. Open the `BluetoothJammer.imo` file, plugin your ESP32, select your port/board and click upload.

## Contributing

Pull requests are welcome. Please open an issue if you find any problems.

## License

[MIT](https://choosealicense.com/licenses/mit/)