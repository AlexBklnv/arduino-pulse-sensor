#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10
#define PULSE_PIN 0
#define TIME_BEFORE_STOP_SCANNING 10000
#define RADIO_SENDING_INTERVAL 600

RF24 radio(CE_PIN, CSN_PIN);
PulseOximeter pox;

bool isScaningEnabled = true;
bool isSerialEnabled = true;
bool isPoxEnabled = false;
int BPM = 0;
unsigned long intScanTime = 0;
unsigned long lastRadioSendTimeStamp  = 0;
unsigned long lastRadioReceiveTimeStamp = 0;

void setup() {
  if (isSerialEnabled) {
    Serial.begin(115200);
    Serial.println("Initializing MAX30100");
  }
  pox.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);
  switchScanning(false);
  initRadio();
  if (isSerialEnabled)
    Serial.println("OK");
}

void loop() {
  readCommandFromRadio();
  if (!isScaningEnabled) {
    lastRadioSendTimeStamp = millis();
  }

  if (isScaningEnabled) {
    if (millis() - lastRadioReceiveTimeStamp >= TIME_BEFORE_STOP_SCANNING) {
      switchScanning(false);
    }
  }

  if (isScaningEnabled) {
    if (!isPoxEnabled) {
      switchScanning(true);
    } else {
      pox.update();
      if (millis() - lastRadioSendTimeStamp >= RADIO_SENDING_INTERVAL) {
        BPM = (int) pox.getHeartRate();
        if (BPM > 30 && BPM < 300) {
          writeDataToRadio();
          lastRadioSendTimeStamp = millis();
        }
      }
    }
  }
}

void switchScanning(bool scanStatus) {
  if (scanStatus) {
    pox.enableScan();
    isPoxEnabled = true;
    if (isSerialEnabled)
      Serial.println("Scanning started!");
  }
  if (!scanStatus) {
    pox.disableScan();
    isScaningEnabled = false;
    isPoxEnabled = false;
    if (isSerialEnabled)
      Serial.println("Scanning stoped!");
  }
}

void onBeatDetected() {
  if (isSerialEnabled) {
    Serial.println("Beat!");
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.print("% / temp:");
    Serial.print(pox.getTemperature());
    Serial.println("C");
  }
}

