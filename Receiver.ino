#include <SoftwareSerial.h>
#include <Servo.h>

// HC-12 connected to pins 10 (TX) and 11 (RX)
SoftwareSerial HC12(10, 11);

// L298N Motor Driver Pins
const int motor1Pin1 = 5;  // Motor 1 IN1
const int motor1Pin2 = 6;  // Motor 1 IN2
const int motor2Pin1 = 7;  // Motor 2 IN3
const int motor2Pin2 = 8;  // Motor 2 IN4
const int ENA = 9;         // Speed control for Motor 1
const int ENB = 3;         // Speed control for Motor 2

// Servo Motors
Servo servo1, servo2, servo3, servo4;

void setup() {
  HC12.begin(9600);   // Start HC-12 communication
  Serial.begin(9600); // Start serial monitor for debugging

  // Configure motor driver pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT); 

  // Attach servos to their respective pins
  servo1.attach(4);
  servo2.attach(12);
  servo3.attach(A0);
  servo4.attach(A1);
  
  Serial.println("Receiver ready!");
}

void loop() {
  // Check if data is available from the transmitter
  if (HC12.available()) {
    String data = HC12.readStringUntil('\n'); // Read incoming data
    Serial.println("Received: " + data); // Debug output

    // Split the data into individual values
    int values[6];
    int index = 0;
    char* token = strtok((char*)data.c_str(), ",");
    while (token != NULL && index < 6) {
      values[index++] = atoi(token);
      token = strtok(NULL, ",");
    }

    // Check if data was correctly parsed
    if (index == 6) {
      Serial.println("Parsed values: ");
      for (int i = 0; i < 6; i++) {
        Serial.print(values[i]);
        Serial.print(" ");
      }
      Serial.println();

      // Control the car and servos with parsed values
      controlCar(values[0], values[1]);                  // Joystick 1 for DC motors
      controlServos(values[2], values[3], values[4], values[5]); // Joystick 2 & 3 for servos
    } else {
      Serial.println("Error: Incomplete data received.");
    }
  }
}

// DC Motor Control with Forward, Backward, and Turning Logic
void controlCar(int joystickX, int joystickY) {
  int deadZone = 50;            // Small range where joystick input is ignored
  int speedY = joystickY - 512; // Forward/Backward control (Y-axis)
  int speedX = joystickX - 512; // Turning control (X-axis)

  // Constrain values to avoid exceeding speed limits
  speedY = constrain(speedY, -511, 511);
  speedX = constrain(speedX, -511, 511);

  // Move Forward/Backward based on Y-axis input
  if (abs(speedY) > deadZone) {
    int speed = map(abs(speedY), 0, 511, 255, 255);
    if (speedY > 0) { // Move forward
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
    } else { // Move backward
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
    }
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    Serial.print("Moving with speed: ");
    Serial.println(speed);
  }
  // Turn Left/Right based on X-axis input
  else if (abs(speedX) > deadZone) {
    int speed = map(abs(speedX), 0, 511, 255, 255);
    if (speedX > 0) { // Turn right
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
    } else { // Turn left
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
    }
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    Serial.print("Turning with speed: ");
    Serial.println(speed);
  }
  // Stop motors if joystick is in the dead zone
  else {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    Serial.println("Motors stopped.");
  }
}

// Servo Control with Joysticks 2 and 3
void controlServos(int joy2X, int joy2Y, int joy3X, int joy3Y) {
  // Map joystick values to servo angles (0-180 degrees)
  servo1.write(map(joy2X, 0, 1023, 0, 180));
  servo2.write(map(joy2Y, 0, 1023, 0, 180));
  servo3.write(map(joy3X, 0, 1023, 0, 180));
  servo4.write(map(joy3Y, 0, 1023, 0, 180));
}
