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

void greeneyLightsOn() {
  digitalWrite(13, HIGH);
  lightOn = 1;
  firstOn = 0;
  firstOff = 1;
  Serial.print(hour()); Serial.print(" "); Serial.print(minute()); Serial.print(" "); Serial.print(second());
      Serial.print(" "); Serial.print(day()); Serial.print(" "); Serial.print(month()); Serial.print(" "); Serial.print(year()); Serial.print(" "); Serial.println("LIGHTS ON");
}

void greeneyLightsOff() {
  digitalWrite(13, LOW);
  lightOn = 0;
  firstOn = 1;
  firstOff = 0;
  Serial.print(hour()); Serial.print(" "); Serial.print(minute()); Serial.print(" "); Serial.print(second());
      Serial.print(" "); Serial.print(day()); Serial.print(" "); Serial.print(month()); Serial.print(" "); Serial.print(year()); Serial.print(" "); Serial.println("LIGHTS OFF");
}

void greeneySetUp() {
  Serial.println("55 - SET DATE (ho mi se da mo ye)");
  temp[1] = readData();
  temp[2] = readData();
  temp[3] = readData();
  temp[4] = readData();
  temp[5] = readData();
  temp[6] = readData();
  timeOffset = now();
  greeneySetTime(temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]);

  Serial.println("52 - SET DAY HOURS");
  d = readData();
  interval1 = (3600*d);
  Serial.println("53 - SET NIGHT HOURS");
  n = readData();
  interval2 = (3600*n);
}

char readData() {
  Serial.println("w");
  while (1) {
    if (Serial.available() > 0) {
      return Serial.parseInt();
    }
  }
}
