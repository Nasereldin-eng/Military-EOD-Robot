#include <SoftwareSerial.h>

// HC-12 connected to pins 10 (TX) and 11 (RX)
SoftwareSerial HC12(10, 11);

// Joystick pins
const int joy1X = A0;
const int joy1Y = A1;
const int joy2X = A2;
const int joy2Y = A3;
const int joy3X = A4;
const int joy3Y = A5;

void setup() {
  HC12.begin(9600); // Start HC-12 communication
  Serial.begin(9600); // Serial monitor for debugging

  // Configure joystick pins as inputs
  pinMode(joy1X, INPUT);
  pinMode(joy1Y, INPUT);
  pinMode(joy2X, INPUT);
  pinMode(joy2Y, INPUT);
  pinMode(joy3X, INPUT);
  pinMode(joy3Y, INPUT);
}

void loop() {
  // Read joystick values (0-1023 range)
  int joy1XVal = analogRead(joy1X);
  int joy1YVal = analogRead(joy1Y);
  int joy2XVal = analogRead(joy2X);
  int joy2YVal = analogRead(joy2Y);
  int joy3XVal = analogRead(joy3X);
  int joy3YVal = analogRead(joy3Y);

  // Send data as a comma-separated string
  String data = String(joy1XVal) + "," + String(joy1YVal) + "," +
                String(joy2XVal) + "," + String(joy2YVal) + "," +
                String(joy3XVal) + "," + String(joy3YVal);
                
  HC12.println(data); // Send the joystick data to the receiver
  Serial.println("Sending: " + data); // Debug output
  
  delay(100); // Delay to prevent flooding[cite: 4]
}
