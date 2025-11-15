#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic *pCharacteristic;

void setup()
{
  Serial.begin(115200);

  BLEDevice::init("ESP32-WebBLE");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");

  pCharacteristic = pService->createCharacteristic(
      "abcdefab-1234-5678-1234-abcdefabcdef",
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Hello from ESP32!");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("12345678-1234-1234-1234-1234567890ab");
  BLEDevice::startAdvertising();

  Serial.println("BLE started, waiting for connection...");
}

void loop()
{
  // nothing needed
}
