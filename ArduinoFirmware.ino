// Firmware for the Arduino Uno in the Lava Viscometer
// Fall 2023 Senior Design Project
// Michael Dodge II

#include "./RunExperiment.h"

#define greenButtonPin       2  // Digital pin for the green button
#define redButtonPin         3  // Digital pin for the red button
#define shutdownButtonPin    4  // Digital pin for the shutdown button
#define torqueSensorPin      A0 // Analog pin for the torque sensor
#define temperatureSensorPin A2 // Analog pin for the temperature sensor

#define rotationDirectionPin 5  // Digital PWM pin for the rotation motor's direction
#define rotationPulsePin     6  // Digital PWM pin for the rotation motor's motion
#define linearDirectionPin   9  // Digital PWM pin for the linear motor's direction
#define linearPulsePin       10 // Digital PWM pin for the linear motor's motion

#define rotationPulseRate 150 // Hertz
#define linearPulseRate   150 // Hertz

#define timeBetweenRotationDirectionToggles 1000 // Milliseconds
#define timeBetweenLinearDirectionToggles   1000 // Milliseconds

DynamicJsonDocument status(128);

void setup() {
  // Start serial port at given baudrate
  Serial.begin(38400);
  // Set pin modes
  pinMode(greenButtonPin, INPUT);
  pinMode(redButtonPin, INPUT);
  pinMode(shutdownButtonPin, INPUT);
  pinMode(torqueSensorPin, INPUT);
  pinMode(temperatureSensorPin, INPUT);
  pinMode(rotationDirectionPin, OUTPUT);
  pinMode(rotationPulsePin, OUTPUT);
  pinMode(linearDirectionPin, OUTPUT);
  pinMode(linearPulsePin, OUTPUT);
}

void loop() {
  if (!digitalRead(greenButtonPin)) {
    uint8_t experimentExitCode = runExperiment(
     greenButtonPin,
     redButtonPin,
     shutdownButtonPin,
     torqueSensorPin,
     temperatureSensorPin,
     rotationPulseRate,
     linearPulseRate,
     timeBetweenRotationDirectionToggles,
     timeBetweenLinearDirectionToggles,
     rotationDirectionPin,
     rotationPulsePin,
     linearDirectionPin,
     linearPulsePin
    );
    switch (experimentExitCode) {
      case 0: // red button
        break;
      case 1: // shutdown button
        while(true) {
          status["msg"] = "Shutdown pending.";
          serializeJson(status, Serial);
          delay(1000);
        }
      default: // unknown error
        while(true){
          Serial.println("Something bad happened.");
          delay(1000);
        }
    }
  }

}
