#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9                      // пин радиомодуля Chip Enable
#define CSN_PIN 10                    // пин радиомодуля Chip Select
#define PULSE_PIN 0                   // пин сенсора пульса
#define LED_PIN 13                    // встроенный лед 

RF24 radio(CE_PIN, CSN_PIN);

bool  startScan = false;

unsigned long intScanTime = 0;                  // следим за интервалом в 2 мс
unsigned long lastRadioSendTimeStamp  = 0;      // передача данных интервал
unsigned long lastRadioReceiveTimeStamp = 0;


// переменные по обработке пульса
int BPM = 1234;                           // Удары в минуту
int Signal;                          // Сигнал получаемый с датчика пульса
int IBI = 600;                       // Интервал между ударами, подстраиваемая переменная
boolean Pulse = false;               // "True" при обнаружении сердцебиения, используется в прерывании
boolean QS = false;                  // Если есть промежуток QRS то True есть пульс

void setup() {
  pinMode(LED_PIN, OUTPUT);        // настраиваем лед для моргании при биении сердца
  Serial.begin(115200);
  // если питание меньше 5v то раскомментить
  // analogReference(EXTERNAL);
  initRadio();
}

void loop() {
  // отправка уровня сигнала в сериал
  // serialOutputSignal();

  readDataFromRadio();

  if (!startScan) {
    lastRadioSendTimeStamp = millis();
    Serial.print("Start\r\n");
  }

  if (startScan) {
    if (millis() - lastRadioReceiveTimeStamp >= 300000) {
      startScan = false;
      Serial.print("Stop\r\n");
    }
  }

  if (startScan) {
    if (millis() - intScanTime >= 2) {
      scanning();
    }
    if (millis() - lastRadioSendTimeStamp >= 1000) {
      if (QS == true) {                  // если есть пульс то срабатывает
        //serialOutputWhenBeatHappens();   // отправка в сериал данных о пульсе, время между ударами и сердцебиение
        QS = false;
        writeDataToRadio();
        lastRadioSendTimeStamp = millis();
      }
    }
  }
}
