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
  digitalWrite(pinArray[0], HIGH);
  //Serial.print("ID: "); Serial.println(Alarm.getTriggeredAlarmId());
  Alarm.disable(0); Alarm.disable(1);
  interval1 = (60 * d); // interval1 = (3600 * d);
  interval2 = (60 * n); // interval2 = (3600 * n);
  if (!alarmOffDisable) {
    Alarm.write(0, interval1); Alarm.enable(0);
  }
  //Alarm.timerOnce(interval1, greeneyLightsOff);
  lightOn = 1;
  /*
    firstOn = 0;
    firstOff = 1;
  */
  Serial.print(hour()); Serial.print("\t"); Serial.print(minute()); Serial.print("\t"); Serial.print(second());
  Serial.print("\t"); Serial.print(day()); Serial.print("\t"); Serial.print(month()); Serial.print("\t"); Serial.print(year()); Serial.print("\t"); Serial.println("LIGHTS ON");
}

void greeneyLightsOff() {
  digitalWrite(pinArray[0], LOW);
  //Serial.print("ID: "); Serial.println(Alarm.getTriggeredAlarmId());
  Alarm.disable(0); Alarm.disable(1);
  interval1 = (60 * d); // interval1 = (3600 * d);
  interval2 = (60 * n); // interval2 = (3600 * n);
  if (!alarmOnDisable) {
    Alarm.write(1, interval2); Alarm.enable(1);
  }
  //Alarm.timerOnce(interval2, greeneyLightsOn);
  lightOn = 0;
  /*
    firstOn = 1;
    firstOff = 0;
  */
  Serial.print(hour()); Serial.print("\t"); Serial.print(minute()); Serial.print("\t"); Serial.print(second());
  Serial.print("\t"); Serial.print(day()); Serial.print("\t"); Serial.print(month()); Serial.print("\t"); Serial.print(year()); Serial.print("\t"); Serial.println("LIGHTS OFF");
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
}

char readData() {
  Serial.println("w");
  while (1) {
    Alarm.delay(0);
    if (Serial.available() > 0) {
      return Serial.parseInt();
    }
  }
}
