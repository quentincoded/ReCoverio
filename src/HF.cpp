// HF.cpp
#include "HF.h"
#include <Arduino.h>
// #include <Adafruit_BNO055.h>
#include <Wire.h>
#include <SparkFun_VL6180X.h>



VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);
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
  Serial.println("Initializing BLE..."); // Add this
  BLEDevice::init("ReCover"); // Set the device name
  Serial.println("BLEDevice initialized.");
  Serial.println("BLE server created.");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  Serial.println("BLE server callbacks set.");

  auto* service = pServer->createService(SERVICE_UUID);
  Serial.println("BLE service created."); // Add this
  pCharacteristic = service->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  Serial.println("BLE characteristic created."); // Add this
  pCharacteristic->addDescriptor(new BLE2902());
  Serial.println("BLE descriptor added."); // Add this
  service->start();
  Serial.println("BLE service started.");

  auto* adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  adv->setMinPreferred(0x06);
  adv->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE advertising started.");
}



void handleBLE() {
  // if (!deviceConnected) return;

  // 1) Read all the vectors
  
  int fsr_value=analogRead(FSR_PIN);
  int pot_value = analogRead(POT_PIN);
  int distance = sensor.getDistance();


  //print the values
  // Serial.print("FSR Value: ");
  // Serial.print(fsr_value);
  // Serial.print(" | Potentiometer Value: ");
  // Serial.println(pot_value);

  if (!deviceConnected) {
    delay(1000); // Still delay even if not connected, to avoid spamming Serial too fast
    Serial.println("No client connected.");
    return;     // Exit if no client is connected
  }

  

  // 2) Pack into a single float buffer:
  float payload[3] = {0}; // Adjust size as needed
  //radom values for testing
    // fsr_value = 1;
    // pot_value = 2;
    // distance = 3;
    payload[0] = (float)fsr_value; // Put FSR value in the first slot
    payload[1] = (float)pot_value; // Example for potentiometer
    payload[2] = (float)distance;

    // quat.w(), quat.x(), quat.y(), quat.z(),
    // linAccel.x(), linAccel.y(), linAccel.z(),
    // mag.x(), mag.y(), mag.z()
    

  // 3) Send raw bytes in one notification
  pCharacteristic->setValue((uint8_t*)payload, sizeof(payload));
  pCharacteristic->notify(); //sends the data to the client
  // Serial.println("BLE notification sent.");

  // 4) Print a CSV debug line
  Serial.printf(
    "FSR: %f\t|\tPOT: %f\t|\tTOF: %f\n",
    payload[0], // FSR value
    payload[1], // POT value
    payload[2]  // TOF value
  );

  // 5) Add a delay!

  delay(100); // Example: delay 1000ms between readings/sends
}

