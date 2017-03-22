const uint64_t pipes[2] = { 0xABCDABCD71, 0x544d52687C };

void initRadio() {
  radio.begin();
  delay(2);
  radio.setChannel(85);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[0]);                // Открываем канал передачи
  radio.openReadingPipe(1, pipes[1]);             // Открываем канал приема
  radio.startListening();
  radio.printDetails();
}

void writeDataToRadio() {                         // отправка данных на велопк
  if (BPM > 20 && BPM < 300) {
    char msg[3];
    sprintf(msg, "%d", BPM);
    radio.stopListening();
    radio.write(&msg, sizeof(msg));
    radio.startListening();
  }
}

void readCommandFromRadio() {                     // прием команд от вело пк
  if (radio.available()) {
    char msg[3];
    radio.read(&msg, 3);
    if (msg[0] == '#') {
      if (msg[1] == '<' && msg[2] == '<') {
        lastRadioReceiveTimeStamp = millis();
        startScan = true;
      } else if (msg[1] == '>' && msg[2] == '|') {
        startScan = false;
      }
    }
  }
}
