#ifndef HF_H
#define HF_H
//define in this file the libraries and the variables that are used in the main.cpp, this hf.cpp file and other files

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


// Custom Service and Characteristic UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Pin definitions for sensors (Use const int or #define) const in is better than define for type saftey
const int FSR_PIN = 0;
const int POT_PIN = 1;
const int TOF_PIN = 32; // Make sure these pin numbers are valid ADC pins on ESP32-C3!


// External declarations for BLE objects and connection flag
extern bool deviceConnected;
extern BLEServer* pServer;
extern BLECharacteristic* pCharacteristic;


//declarations for IMU data


// Function prototypes for initializing BLE and handling events
void initBLE();
void handleBLE();

#endif //HF_H