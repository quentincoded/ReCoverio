// HF.cpp
#include "HF.h"
#include <Arduino.h>
// #include <Adafruit_BNO055.h>
#include <Wire.h>



// These symbols are defined in your main sketch:
// extern Adafruit_BNO055 bno;
//extern const int       BNO055_SAMPLERATE_DELAY_MS;

// BLE globals
bool               deviceConnected = false;
BLEServer*         pServer         = nullptr;
BLECharacteristic* pCharacteristic = nullptr;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* srv) override {
    deviceConnected = true;
    Serial.println("BLE client connected.");
  }
  void onDisconnect(BLEServer* srv) override {
    deviceConnected = false;
    Serial.println("BLE client disconnected.");
    BLEDevice::startAdvertising();
  }
};

void initBLE() {
  BLEDevice::init("Recover");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  auto* service = pServer->createService(SERVICE_UUID);
  pCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());
  service->start();

  auto* adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  adv->setMinPreferred(0x06);
  adv->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE advertising started.");
}

void handleBLE() {
  if (!deviceConnected) return;

  // 1) Read all the vectors
  
  int fsr_value=analogRead(FSR_PIN);
  int pot_value = analogRead(POT_PIN);
  

  // 2) Pack into a single float buffer:
  float payload[3] = {0}; // Adjust size as needed

    payload[0] = (float)fsr_value; // Put FSR value in the first slot
    payload[1] = (float)pot_value; // Example for potentiometer
    payload[2] = 0.0f; // Placeholder for TOF value
    // quat.w(), quat.x(), quat.y(), quat.z(),
    // linAccel.x(), linAccel.y(), linAccel.z(),
    // mag.x(), mag.y(), mag.z()
    

  // 3) Send raw bytes in one notification
  pCharacteristic->setValue((uint8_t*)payload, sizeof(payload));
  pCharacteristic->notify(); //sends the data to the client
  // Serial.println("BLE notification sent.");

  // 4) Print a CSV debug line
  Serial.printf(
    "Q:%.3f,%.3f,%.3f,%.3f | A:%.2f,%.2f,%.2f | M:%.1f,%.1f,%.1f\n",
    payload[0], payload[1], payload[2], payload[3],
    payload[4], payload[5], payload[6],
    payload[7], payload[8], payload[9]
  );

  // 5) Add a delay!
  // 10HZ
  delay(100); // Example: delay 100ms between readings/sends
}
