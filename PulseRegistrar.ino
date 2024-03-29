/*
unsigned long sampleCounter = 0;           // используется для определения интервала сердцебиения
unsigned long lastBeatTime = 0;            // используется для определения интервала сердцебиения
int P = 512;                               // используется для нахождения верхней границы пульсовой волны
int T = 512;                               // используется для нахождения нижней границы пульсовой волны
int thresh = 525;                          // используется для нахождения мгновенного момента удара сердца
boolean firstBeat = true;                  // для нахождения BPM

void scanning() {
  Signal = analogRead(PULSE_PIN);              // считтываем сигнал с сенсора
  sampleCounter += 2;                         // отслеживаем время в ms
  int N = sampleCounter - lastBeatTime;       // отслеживаем время между ударами для отсеивания шума

  //  находим верхнюю и нижнию границы волны
  if (Signal < thresh && N > (IBI / 5) * 3) { // избегаем шума ожидая интервала 3/5 от последнего IBI
    if (Signal < T) {
      T = Signal;
    }
  }
  if (Signal > thresh && Signal > P) {
    P = Signal;
  }

  if (N > 250) {                                  // избегаем высокочастотного шума
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3)
         && (Signal > 512 + 25 || Signal < 512 - 25) && (Signal > 224 && Signal < 800)) { // делаем жесткую границу считывания пульса при которых обрабатывается сигнал
      // позволяет ненмого избежать лишних шумов на линии
      Pulse = true;                               // пульс есть
      IBI = sampleCounter - lastBeatTime;         // измеряем время между ударами в ms
      lastBeatTime = sampleCounter;               // сохраняем момент последнего удара

      if (firstBeat) {                            // если мы первый раз обнаружили удар
        firstBeat = false;                        // меняем значение для второго удара
        intScanTime = millis();                              // включаем прерывание
        return;
      }

      BPM = 60000 / IBI;                          // вычисление BPM. 60 сек делим на время последнего удара
      QS = true;
    }
  }

  if (Signal < thresh && Pulse == true) {  // когда значения понижаются, то ритм кончился
    Pulse = false;                         // меняем значене отсутствие пульса
    thresh = (P - T) / 2 + T;                  // устанавливаем в значение 50% от волны (амплитуда)/2 + Т
    P = thresh;                            // сбрасываем значение волны
    T = thresh;
  }

  if (N > 2500) {                          // если 2,5мс без единого удара
    thresh = 512;                          // сбрасываем на дефолт
    P = 512;
    T = 512;
    lastBeatTime = sampleCounter;          // устанавливаем значение последнего удара
    firstBeat = true;
  }
  intScanTime = millis();
}

*/


