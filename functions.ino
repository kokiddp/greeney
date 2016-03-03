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
  lightOn = 1;
  Serial.print(hour()); Serial.print("\t"); Serial.print(minute()); Serial.print("\t"); Serial.print(second());
  Serial.print("\t"); Serial.print(day()); Serial.print("\t"); Serial.print(month()); Serial.print("\t"); Serial.print(year()); Serial.print("\t"); Serial.println("LIGHTS ON");
  EEPROM.write(4, lightOn);
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
  lightOn = 0;
  Serial.print(hour()); Serial.print("\t"); Serial.print(minute()); Serial.print("\t"); Serial.print(second());
  Serial.print("\t"); Serial.print(day()); Serial.print("\t"); Serial.print(month()); Serial.print("\t"); Serial.print(year()); Serial.print("\t"); Serial.println("LIGHTS OFF");
  EEPROM.write(4, lightOn);
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
  // Serial.println("len(pinArray)");
  int cmd = readData();
  for (int i = 0; i < cmd; i++) {
    // Serial.print("pinArray["); Serial.print(i); Serial.println("]");
    pinArray[i] = readData();
    pinMode(pinArray[i], OUTPUT);
  }
  //temp[0] = readData();
  //if (temp[0] == 1) {
  if (EEPROM.read(2) == 1) {
    restore = true;
  }
  else {
    restore = false;
  }
}

void greeneyRestore() {
  d =  EEPROM.read(0);
  n =  EEPROM.read(1);
  if (EEPROM.read(3) == 1){
    greeneyAutomationOn();
  }
  else {
    greeneyAutomationOff();
  }
  if (EEPROM.read(4) == 1){
    greeneyLightsOn();
  }
  else {
    greeneyLightsOff();
  }
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

void greeneyAutomationOn() {
  automation = true;
  EEPROM.write(3, 1);
  /*if (restore) {
    interval1 = (60 * d); //interval1 = (3600 * d);
    interval2 = (60 * n); //interval2 = (3600 * n);
  }
  else {
    d = readData();
    interval1 = (60 * d); //interval1 = (3600 * d);
    n = readData();
    interval2 = (60 * n); //interval2 = (3600 * n);
  }
  */

  interval1 = (60 * d); //interval1 = (3600 * d);
  interval2 = (60 * n); //interval2 = (3600 * n);

  if (lightOn == 1) {
    Alarm.write(0, interval1); Alarm.enable(0);
  }
  else {
    Alarm.write(1, interval2); Alarm.enable(1);
  }
  Alarm.write(1, interval2); Alarm.enable(1);
  alarmOnDisable = false;
  alarmOffDisable = false;
}

void greeneyAutomationOff() {
  automation = false;
  EEPROM.write(3, 0);
  Alarm.disable(0);
  Alarm.disable(1);
  alarmOnDisable = true;
  alarmOffDisable = true;
}
