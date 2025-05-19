/*
  Code for my Bluetooth Jammer on an ESP32 with 2x NRF24L01 modules.
  More info: https://github.com/stuthemoo/ESP32BluetoothJammer

  FOR EDUCATIONAL PURPOSES ONLY!
  This code repository is for educational purposes only! DO NOT use a signal jamming device as they can can interfere with public safety communications and emergency services. In many countries the use of signal jammers is prohibited by law and intentional interference with wireless signals can result in severe penalties.
*/

#include <SPI.h>
#include <RF24.h>

#define BUTTON_PIN 33
#define LED_PIN 25

// If you don't have an on/off button then change this to false!
boolean hasButton = true;

// First NRF24L01 on CE 27, CSN 32
RF24 radio1(27, 32);

// Second NRF24L01 on CE 4, CSN 2
RF24 radio2(4, 2);

// const byte addresses[][6] = {"00001", "00002"};

// Bluetooth channels range - Half for each RF module
const uint8_t startCh1 = 2;    // 2402 MHz
const uint8_t endCh1   = 41;   // 2441 MHz
const uint8_t startCh2 = 42;   // 2442 MHz
const uint8_t endCh2   = 80;   // 2480 MHz

// State
boolean jammingOn = false;
boolean buttonState = HIGH;
boolean lastButtonState = HIGH;

void setup() {
  // Start serial monitor for debugging
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("BLE Jammer starting...");
  delay(1000);

  // Check radios have started ok
  if (!radio1.begin()) {
    Serial.println("Radio 1 init failed. Check wiring.");
    while (1);
  }
  if (!radio2.begin()) {
    Serial.println("Radio 2 init failed. Check wiring.");
    while (1);
  }

  // Configure radio modules settings
  setupRadio(radio1);
  setupRadio(radio2);

  // Button & LED pins
  if(hasButton) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
  }else{
    // Start jamming automatically if there is no button
    jammingOn = true;
  }

  Serial.println("Setup complete.");
}

void setupRadio(RF24& radio) {
  // Radio settings
  // radio.openWritingPipe(addresses[0]); // 00001
  radio.setAutoAck(false);            // Disable auto acknowledgement
  radio.setRetries(0, 0);             // No retries
  radio.setDataRate(RF24_2MBPS);      // High rate to increase traffic
  radio.setPALevel(RF24_PA_MAX);      // Max power
  radio.disableCRC();                 // Disable CRC validation
  radio.setPayloadSize(32);           // Standard BLE packet size
}

void loop() {
  // Set starting channel of each antenna
  static uint8_t ch1 = startCh1;
  static uint8_t ch2 = startCh2;
  
  if(hasButton) {
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
  }

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

    // Radio 1 transmit random data
    radio1.setChannel(ch1);
    radio1.stopListening();
    radio1.write(&payload1, sizeof(payload1));
    Serial.println("Radio 1 transmit on channel " + String(ch1));
    
    // Switch radio 1 to next channel
    ch1++;
    if (ch1 > endCh1) ch1 = startCh1;

    // Radio 2 transmit random data
    radio2.setChannel(ch2);
    radio2.stopListening();
    radio2.write(&payload2, sizeof(payload2));
    Serial.println("Radio 2 transmit on channel " + String(ch2));

    // Switch radio 2 to next channel
    ch2++;
    if (ch2 > endCh2) ch2 = startCh2;

    // Short delay to avoid locking up
    delayMicroseconds(300);
  }
}