// Firmware for the Arduino Uno in the Lava Viscometer
// Fall 2023 Senior Design Project
// Michael Dodge II

#include "./ArduinoJson.h"

#define greenButtonPin       2;  // Digital pin for the green button
#define redButtonPin         4;  // Digital pin for the red button
#define shutdownButtonPin    6;  // Digital pin for the shutdown button
#define torqueSensorPin      A0; // Analog pin for the torque sensor
#define temperatureSensorPin A2; // Analog pin for the temperature sensor

#define linearActuator

uint32_t cycle = 0; // How many times the loop has completed

DynamicJsonDocument data(1024);
/*
 * unsigned 32-bit int c: the amount of cycles since the start of the operational period
 * float t: the amount of time since the start of the operational period
 * int gb: the boolean state of the digital pin for the green button
 * int rb: the boolean state of the digital pin for the red button
 * int sdb: the boolean state of the digital pin for the shutdown button
 * int 
 */

void setup() {
  // Start serial port at given baudrate
  Serial.begin(9600);
  // Set pin modes
  pinMode(greenButtonPin, INPUT)
  pinMode(redButtonPin, INPUT)
  pinMode(shutdownButtonPin, INPUT)
  pinMode(torqueSensorPin, INPUT)
  pinMode(temperatureSensorPin, INPUT)
}

void loop() {
  // Get current cycle and time stamp
  data["c"] = cycle;
  data["t"] = millis();
  
  // Get state of buttons
  data["gb"] = (bool)digitalRead(greenButtonPin);
  data["rb"] = (bool)digitalRead(redButtonPin);
  data["sdb"] = (bool)digitalRead(shutdownButtonPin);

  // Get sensor values: raw 10-bit form (as the Pi is better-equipped for math)
  data["tq"] = analogRead(torqueSensorPin);
  data["tm"] = analogRead(temperatureSensorPin);

  // Print to Serial Port
  serializeJson(data, Serial);
  Serial.print("\n");
  
  cycle += 1;
}