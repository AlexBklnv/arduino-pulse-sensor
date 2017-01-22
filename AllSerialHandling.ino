void serialOutputSignal() {
  sendDataToSerial('S', Signal);
}

void serialOutputWhenBeatHappens() {
  sendDataToSerial('B', BPM);  // ударов в минуту
  sendDataToSerial('Q',IBI);   // интервал между ударами
}

void sendDataToSerial(char symbol, int data ) {
  Serial.print(symbol);
  Serial.println(data);
}

String serialReceiveMessage() {
  String rMessage = "";           // Временная переменная хранит полученное сообщение
  char receivedChar;              // Переменная для корректной обработки полученого символа
  while (Serial.available()) {
    receivedChar = Serial.read();
    rMessage += receivedChar;
    delay(3);

    if (rMessage.length() == 10)
      break;
  }
  return rMessage;
}


