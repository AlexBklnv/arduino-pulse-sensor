const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

void initRadio() {
  radio.begin();
  delay(2);
  radio.setChannel(91);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(7, 15);
  radio.openWritingPipe(pipes[0]);               // Открываем канал передачи
  radio.openReadingPipe(1, pipes[1]);             // Открываем канал приема
  radio.startListening();
}

void writeDataToRadio() {
  String msg = "#";
  msg += BPM;
  radio.write(&msg, sizeof(msg));
}

void readDataFromRadio() {
  if (radio.available()) {
    String msg = "";
    radio.read(&msg, 3);
    if (msg[0] == '#') {
      if (msg[1] == '<' && msg[2] == '<') {
        startScan = true;
      } else if (msg[1] == '>' && msg[2] == '|') {
        startScan = false;
      }
    }
  }
}
