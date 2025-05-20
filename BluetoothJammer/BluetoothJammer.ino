/*
  Code for my Bluetooth Jammer built with an ESP32 with 2x NRF24L01 modules.

  More info: https://github.com/stuthemoo/ESP32BluetoothJammer

  FOR EDUCATIONAL PURPOSES ONLY!
  This code repository is for educational purposes only! DO NOT use a signal jamming device as they can can interfere with public safety communications and emergency services. In many countries the use of signal jammers is prohibited by law and intentional interference with wireless signals can result in severe penalties.
*/

#include <SPI.h>
#include <RF24.h>

#define BUTTON_PIN 0 // Built in BOOT button
#define LED_PIN 2 // Built in LED

// 16 Mhz SPI speed
constexpr int SPI_SPEED = 16000000;

// First NRF24L01 on CE 27, CSN 32
RF24 radio1(22, 21, SPI_SPEED);

// Second NRF24L01 on CE 4, CSN 2
RF24 radio2(16, 15, SPI_SPEED);

// Bluetooth channels - Each transmitter loops through half each
int bluetooth_channels[] = {32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80};

const uint8_t startCh1 = 0;    // 2402 MHz
const uint8_t endCh1   = 9;   // 2441 MHz
const uint8_t startCh2 = 11;   // 2442 MHz
const uint8_t endCh2   = 20;   // 2480 MHz

// State
boolean jammingOn = false;
boolean buttonState = HIGH;
boolean lastButtonState = HIGH;

void setup() {
  // Give ESP32 extra time to start up
  delay(1000);

  // Start serial monitor for debugging
  Serial.begin(115200);
  Serial.println();
  Serial.println("BLE Jammer starting...");
  
  // Give radios extra time to start up
  delay(1000);

  // Check radios have started ok
  if (!radio1.begin()) {
    Serial.println("Radio 1 init failed. Check wiring.");
    while (1); // Stop until reboot
  }
  if (!radio2.begin()) {
    Serial.println("Radio 2 init failed. Check wiring.");
    while (1); // Stop until reboot
  }

  // Configure radio modules settings
  setupRadio(radio1);
  setupRadio(radio2);

  // Setup Button & LED
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Setup complete.");
}

void setupRadio(RF24& radio) {
  // Radio settings
  radio.setAutoAck(false);                // Disable auto acknowledgement
  radio.setRetries(0, 0);                 // No retries
  radio.setDataRate(RF24_2MBPS);          // High rate to increase traffic
  radio.setPALevel(RF24_PA_MAX);          // Max power
  radio.setCRCLength(RF24_CRC_DISABLED);  // Disable CRC validation
  // radio.disableCRC();
  radio.setPayloadSize(32);               // Standard BLE packet size
  radio.stopListening();                  // Transmit mode
  // radio.setAddressWidth(5);
  // radio.startConstCarrier(RF24_PA_MAX, 2);
}

void loop() {
  // Set starting channel of each antenna
  static uint8_t ch1 = startCh1;
  static uint8_t ch2 = startCh2;

  // Get debugging info using serial monitor
  if (Serial.available() > 0) {
    String serialInput = Serial.readString();
    serialInput.trim();

    // If 'debug1' or 'debug2' is typed then show debugging info for that radio
    if(serialInput == "debug1") {
      Serial.println("Radio 1 debugging info...");
      radio1.printPrettyDetails();
    }else if(serialInput == "debug2") {
      Serial.println("Radio 2 debugging info...");
      radio2.printPrettyDetails();
    }
  }
  
  // If the button is pressed then start/stop jamming
  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == LOW && lastButtonState == HIGH ) {
    if(jammingOn) {
      // Stop jamming
      jammingOn = false;

      // Turn the LED off
      digitalWrite(LED_PIN, LOW);
    }else{
      // Start jamming
      jammingOn = true;

      // Turn the LED on
      digitalWrite(LED_PIN, HIGH);
    }
  }
  lastButtonState = buttonState;

  // If the jammer has been started
  if(jammingOn) {
    // Payloads that will be transmitted
    uint8_t payload1[32];
    uint8_t payload2[32];

    // Fill payloads with random data
    for (int i = 0; i < 32; i++) {
      payload1[i] = random(0, 256);
      payload2[i] = random(0, 256);
    }

    // Radio 1 transmit the random data
    radio1.setChannel(bluetooth_channels[ch1]);
    radio1.write(&payload1, sizeof(payload1));
    Serial.println("Radio 1 transmit on channel " + String(bluetooth_channels[ch1]));
    
    // Switch radio 1 to next channel
    ch1++;
    if (ch1 > endCh1) ch1 = startCh1;

    // Radio 2 transmit the random data
    radio2.setChannel(bluetooth_channels[ch2]);
    radio2.write(&payload2, sizeof(payload2));
    Serial.println("Radio 2 transmit on channel " + String(bluetooth_channels[ch2]));

    // Switch radio 2 to next channel
    ch2++;
    if (ch2 > endCh2) ch2 = startCh2;

    // Short delay to avoid locking up
    delayMicroseconds(300);
  }
}