void setup() {
  Serial.begin(19200);
  Serial2.begin(19200);
}

void loop() { 
  while (Serial2.available()) {
    Serial.write(char(Serial2.read()));
  }
  while (Serial.available()) {
    Serial2.write(char(Serial.read()));
  }
}
