#include <MsTimer2.h>

int pulsePin = 0;                   // Пин сенсора
int blinkPin = 13;                  // Пин диода, если есть пульс, то моргает
String msg = "";                    // Сообщение получаемое от второго МК и в дальнейшем сообщение для отправки
bool isScanPulseAvailable = false;
long receiveTimeOut;

// Переменные изменяемые в прерывании
volatile int BPM;                   // Удары в минуту
volatile int Signal;                // Сигнал получаемый с датчика пульса
volatile int IBI = 600;             // Интервал между ударами, подстраиваемая переменная
volatile boolean Pulse = false;     // "True" при обнаружении сердцебиения, используется в прерывании
volatile boolean QS = false;        // Если есть промежуток QRS то True есть пульс

void setup() {
  pinMode(blinkPin, OUTPUT);        // настраиваем лед для моргании при биении сердца
  Serial.begin(115200);
  interruptSetup();                 //установка прерывания срабатываемого раз в 2мс
  // если питание меньше 5v то раскомментить
  // analogReference(EXTERNAL);
  startScan();
}

void loop() {
  // отправка уровня сигнала в сериал
  serialOutputSignal();
  
 /* if (Serial.available()) {
    msg = serialReceiveMessage();
    if (msg == "#SPA=start") {
      startScan();
    } else if (msg == "#SPA=stop") {
      stopScan();
      QS = false;
    }
    msg = "";
  }*/
  
  if (QS == true) {                  // если есть пульс то срабатывает
    serialOutputWhenBeatHappens();   // отправка в сериал данных о пульсе, время между ударами и сердцебиение
    QS = false;
  }
  delay(20);
}

