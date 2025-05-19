# ESP32 Bluetooth Signal Jammer

Arduino code for a Bluetooth signal jamming device built using an ESP32 MCU and NRF24L01 radio transmitters.

From the "[How to build a Bluetooth Jammer](https://www.youtube.com/@StuJohnsonNZ)" video tutorial on YouTube.

### ⚠️ LEGAL WARNING

This code repository is for educational purposes only! DO NOT use a signal jamming device as they can can interfere with public safety communications and emergency services. In many countries the use of signal jammers is prohibited by law and intentional interference with wireless signals can result in severe penalties.

## Wiring Diagrams

Here's the exact wiring for the breadboard version from the video tutorial.

### Breadboard wiring:

![Breadboard Wiring Diagram](https://github.com/stuthemoo/ESP32BluetoothJammer/raw/main/wiring-breadboard.png "Breadboard Wiring Diagram")

## Usage

Follow the wiring diagram and upload this code to your device use the Arduino IDE with ESP32 support and RF24 library installed.

1. [Download the Arduino IDE here](https://www.arduino.cc/en/software).

2. [Install Arduino-ESP32 support](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

3. Using the Arduino library manager install `RF24 (by TMRh20)`.

4. Open the `BluetoothJammer.imo` file. If your circuit does not have a button then change the variable on line 16 to false.

```
boolean hasButton = false;
```

5. Plugin your ESP32. Select your port/board and click upload.

## Contributing

Pull requests are welcome. Please open an issue if you find any problems.

## License

[MIT](https://choosealicense.com/licenses/mit/)