#ifndef HF_H
#define HF_H
//define in this file the libraries and the variables that are used in the main.cpp, this hf.cpp file and other files

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <SparkFun_VL6180X.h>
#include <Wire.h>


// Custom Service and Characteristic UUIDs
#define SERVICE_UUID        "9f3c872e-2f1b-4c58-bc2a-5a2e2f48f519"
#define CHARACTERISTIC_UUID "2d8e1b65-9d11-43ea-b0f5-c51cb352ddfa"

#define VL6180X_ADDRESS 0x29


extern VL6180xIdentification identification;
extern VL6180x sensor;


// Pin definitions for sensors (Use const int or #define) const in is better than define for type saftey
const int FSR_PIN = 0;
const int POT_PIN = 1;
// const int TOF_PIN = 32; // Make sure these pin numbers are valid ADC pins on ESP32-C3!


// External declarations for BLE objects and connection flag
extern bool deviceConnected;
extern BLEServer* pServer;
extern BLECharacteristic* pCharacteristic;


//declarations for IMU data


// Function prototypes for initializing BLE and handling events
void initBLE();
void handleBLE();

#endif //HF_H