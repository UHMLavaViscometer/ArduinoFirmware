#pragma once
#include "./ArduinoJson.h"
// Module to support the firmware for the Arduino Uno in the Lava Viscometer
// Fall 2023 Senior Design Project
// Michael Dodge II

uint8_t runExperiment(
  int greenButtonPin,
  int redButtonPin,
  int shutdownButtonPin,
  int torqueSensorPin,
  int temperatureSensorPin,
  int rotationPulseRate,
  int linearPulseRate,
  int timeBetweenRotationDirectionToggles,
  int timeBetweenLinearDirectionToggles,
  uint8_t rotationDirectionPin,
  uint8_t rotationPulsePin,
  uint8_t linearDirectionPin,
  uint8_t linearPulsePin) {
  /*
   * Returns 0 if terminated by red button
   * Returns 1 if terminated by unknown/error
   * Returns 2 if terminated by shutdown button
  */
  DynamicJsonDocument data(512);
  /*
   * unsigned 32-bit int c: the amount of cycles since the start of the operational period
   * float t: the amount of time since the start of the operational period
   * int gb: the boolean state of the digital pin for the green button
   * int rb: the boolean state of the digital pin for the red button
   * int sdb: the boolean state of the digital pin for the shutdown button
   * int tq: the raw analog (0-1023) value of the torque sensor
   * int tm: the raw analog (0-1023) value of the temperature sensor
  */
  uint32_t cycle = 0; // How many times the loop has completed
  bool rotationDirection = true; // CW when >= 0, CCW otherwise
  bool linearDirection = true;   // Forward when >= 0, Backward otherwise
  uint32_t lastRotationDirectionToggleTime, lastLinearDirectionToggleTime;
    /* Motor Direction Toggling */
    bool rotationDirectionHasResetThisPeriod = false;
    bool linearDirectionHasResetThisPeriod = false;
  data["rb"] = false;
  data["sdb"] = false;

  while (!data["rb"] && !data["sdb"]) {
    // Get current cycle and time stamp
    data["c"] = cycle;
    data["t"] = (uint32_t)millis();
  
    // Get state of buttons
    data["gb"] = !(bool)digitalRead(greenButtonPin);
    data["rb"] = !(bool)digitalRead(redButtonPin);
    //data["sdb"] = !(bool)digitalRead(shutdownButtonPin);

    // Get sensor values: raw 10-bit form (as the Pi is better-equipped for math)
    data["tq"] = (int)analogRead(torqueSensorPin);
    data["tm"] = (int)analogRead(temperatureSensorPin);


    // Rotation
    if (!rotationDirectionHasResetThisPeriod) {
      rotationDirection = !rotationDirection;
      rotationDirectionHasResetThisPeriod = true;
      lastRotationDirectionToggleTime = data["t"];
      digitalWrite(rotationDirectionPin, rotationDirection);
      data["rd"] = rotationDirection;
    }
    else if ((uint32_t)(data["t"]) - lastRotationDirectionToggleTime >= timeBetweenRotationDirectionToggles) {
      rotationDirectionHasResetThisPeriod = false;
    }

    // Linear
    if (!linearDirectionHasResetThisPeriod) {
      linearDirection = !linearDirection;
      linearDirectionHasResetThisPeriod = true;
      lastLinearDirectionToggleTime = data["t"];
      digitalWrite(linearDirectionPin, linearDirection);
      data["ld"] = linearDirection;
    }
    else{
      if ((uint32_t)(data["t"]) - lastLinearDirectionToggleTime >= timeBetweenLinearDirectionToggles) {
        linearDirectionHasResetThisPeriod = false;
      }
    }



    // Print to Serial Port
    serializeJson(data, Serial);
    Serial.print("\n");
    cycle += 1;
  }

  /* Shutdown Cases */
  if (static_cast<bool>(data["sdb"]) && false) {
    digitalWrite(rotationPulsePin, 0);
    digitalWrite(linearPulsePin, 0);
    return 1;
  }
  else if (static_cast<bool>(data["rb"])) {
    digitalWrite(rotationPulsePin, 0);
    digitalWrite(linearPulsePin, 0);
    return 0;
  }
  else {
    digitalWrite(rotationPulsePin, 0);
    digitalWrite(linearPulsePin, 0);
    return 2;
  }
}