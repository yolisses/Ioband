#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic *pCharacteristic;
BLECharacteristic *pPinCharacteristic;

const int PIN_7 = 7;

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_7, OUTPUT);
  digitalWrite(PIN_7, LOW);

  BLEDevice::init("ESP32-WebBLE");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");

  pCharacteristic = pService->createCharacteristic(
      "abcdefab-1234-5678-1234-abcdefabcdef",
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Hello from ESP32!");

  pPinCharacteristic = pService->createCharacteristic(
      "12345678-abcd-efab-cdef-1234567890ab",
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);

  pPinCharacteristic->setValue("0");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("12345678-1234-1234-1234-1234567890ab");
  BLEDevice::startAdvertising();

  Serial.println("BLE started, waiting for connection...");
}

void loop()
{
  // Check for pin control commands
  std::string pinValue = pPinCharacteristic->getValue();
  if (pinValue.length() > 0)
  {
    if (pinValue[0] == '1')
    {
      digitalWrite(PIN_7, HIGH);
    }
    else if (pinValue[0] == '0')
    {
      digitalWrite(PIN_7, LOW);
    }
  }
  delay(100);
}
