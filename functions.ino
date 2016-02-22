int greeneyMoistSens(int e, int p) {
  digitalWrite(e, HIGH);
  Alarm.delay(2000);
  int ha = analogRead(p);
  digitalWrite(e, LOW);
  int h = (100 - map(ha, 0, 1024, 0, 100));
  return h;
}

void greeneySetTime(int h, int mi, int s, int d, int mo, int y) {
  longTemp[1] = now();
  setTime(h, mi, s, d, mo, y);
  longTemp[2] = now();
  timeOffset = (timeOffset + (longTemp[2] - longTemp[1]));
}

char readData() {
  Serial.println("w");
  while (1) {
    if (Serial.available() > 0) {
      return Serial.parseInt();
    }
  }
}
