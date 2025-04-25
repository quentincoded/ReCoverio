// main.ino
// #include <Wire.h>
#include <Arduino.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BNO055.h>
#include "HF.h"

// Sample rate (in ms) between BLE notifications
//const int BNO055_SAMPLERATE_DELAY_MS = 100;

// Create the sensor instance
// Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);



void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("FSR + POTENTIOMETER + TOF (to be implemented) + BLE Sensor Fusion");

  if (true) {
    Serial.println("Ooops, no BNO055 detected ... Check wiring!");
    
  }

  delay(1000);
  pinMode(FSR_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  // Add pinMode for TOF_PIN if it's a simple digital I/O
  // If TOF_PIN is for I2C/UART etc, its setup is handled by that library's begin()

  // Serial.println("Calibration status: 0=uncal, 3=full");
  int test= 57;
  Serial.print("dinimueter: "); // Use print, not println
  Serial.println(test);        // Use println for the number and newline;

  initBLE();
  Serial.println("BLE initialized.");
}

void loop() {
  handleBLE();
}
