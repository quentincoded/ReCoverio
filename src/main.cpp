// main.ino
#include <Wire.h>
#include <Arduino.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BNO055.h>
#include "HF.h"
#include <SparkFun_VL6180X.h> // Include the library for the VL6180X ToF sensor

// Sample rate (in ms) between BLE notifications
//const int BNO055_SAMPLERATE_DELAY_MS = 100;

// Create the sensor instance
// Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
void printIdentification(struct VL6180xIdentification *temp);


void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");
  while (!Serial) {
    delay(10);
    // Optional: Add a print here to see if it's stuck in the loop
    Serial.println("Waiting for Serial...");
  }
  Serial.println("Serial port is ready.");
  Serial.println("FSR + POTENTIOMETER + TOF ");

  Wire.begin(4, 5);  // GPIO 4 = SDA yellow, GPIO 5 = SCL green  Start I2C library
  delay(100);           // delay .1s
  Serial.println("Wire (I2C) initialized.");
  sensor.getIdentification(&identification); // Retrieve manufacture info from device memory
  printIdentification(&identification);      // Helper function to print all the Module information

  if (sensor.VL6180xInit() != 0)
  {
    Serial.println("Failed to initialize. Freezing..."); // Initialize device and check for errors
    while (1)
      ;
  }

  sensor.VL6180xDefautSettings(); // Load default settings to get started.

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


// Define the printIdentification function here
void printIdentification(struct VL6180xIdentification *temp)
{
  Serial.print("Model ID = ");
  Serial.println(temp->idModel);
  //stuff
}