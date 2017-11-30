// This file was created with help from:
// https://www.allaboutcircuits.com/projects/understanding-and-implementing-the-hc-12-wireless-transceiver-module/
#include <SoftwareSerial.h>

const byte HC12RxdPin = 4;                      // "RXD" Pin on HC12
const byte HC12TxdPin = 10;                     // "TXD" Pin on HC12
const byte HC12SetPin = 6;                      // "SET" Pin on HC12

unsigned long timer = millis();                 // Delay Timer

char HC12ByteIn;                                // Temporary variable
String HC12ReadBuffer = "";                     // Read/Write Buffer 1 for HC12
String SerialReadBuffer = "";                   // Read/Write Buffer 2 for Serial
boolean HC12End = false;                        // Flag to indiacte End of HC12 String

// Software Serial ports Rx and Tx are opposite the HC12 Rx and Tx
// Create Software Serial Port for HC12
SoftwareSerial HC12(HC12TxdPin, HC12RxdPin);

void setup() {
  HC12ReadBuffer.reserve(64);                   // Reserve 64 bytes for Serial message input
  SerialReadBuffer.reserve(64);                 // Reserve 64 bytes for HC12 message input

  pinMode(HC12SetPin, OUTPUT);                  // Output High for Transparent / Low for Command
  digitalWrite(HC12SetPin, HIGH);               // Enter Transparent mode
  delay(80);                                    // 80 ms delay before operation per datasheet
  Serial.begin(9600);                           // Open serial port to computer
  HC12.begin(9600);                             // Open software serial port to HC12

}

void loop() {
  while (HC12.available()) {                    // While Arduino's HC12 soft serial rx buffer has data
    HC12ByteIn = HC12.read();                   // Store each character from rx buffer in byteIn
    HC12ReadBuffer += char(HC12ByteIn);         // Write each character of byteIn to HC12ReadBuffer
    if (HC12ByteIn == '\n') {                   // At the end of the line
      HC12End = true;                           // Set HC12End flag to true
      HC12ReadBuffer.trim();
    }
  }

  while (Serial.available()) {                     // If Arduino's computer rx buffer has data
    String SerialReadBuffer = Serial.readString(); // read the string send over serial
    HC12.print(SerialReadBuffer);                  // Transmit non-command message
  }

  if (HC12End) {                                // If HC12End flag is true
    Serial.print(HC12ReadBuffer + "\n");             // Send message to screen
    HC12ReadBuffer = "";                        // Empty buffer
    HC12End = false;                            // Reset flag
  }

}
