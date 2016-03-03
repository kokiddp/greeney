#ifndef SERIAL_RATE
  #define SERIAL_RATE         9600
#endif

#ifndef SERIAL_TIMEOUT
  #define SERIAL_TIMEOUT      5
#endif

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
//#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <EEPROM.h>

#define DHTPIN            2         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//int BH1750_address = 0x23; // i2c Address
byte buff[2];
//float valLx = 0;
int temp[10];
int ha, h = 0;
int d = 1;
int n = 1;
int command = 99;
long unsigned int timeOffset = 0;
long unsigned int alive = 0;
long unsigned int longTemp[10];
long int interval = 0;
long unsigned int interval1 = 60, interval2 = 60;
int pinArray[100];

boolean alarmOnDisable = true, alarmOffDisable = true, automation = false, restore = false;
byte lightOn = 1;

void setup() {
  /*
    Wire.begin();
    BH1750_Init(BH1750_address);
  */
  Serial.begin(SERIAL_RATE);
  Serial.setTimeout(SERIAL_TIMEOUT);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  greeneySetUp();

  Alarm.timerRepeat(interval1, greeneyLightsOff); //ID 0
  Alarm.timerRepeat(interval2, greeneyLightsOn);  //ID 1

  if (restore) {
    greeneyRestore();
  }
  else {
    greeneyAutomationOff();
    greeneyLightsOn();
  }
}

void loop() {

  Alarm.delay(0);
  
  alive = (now() - timeOffset);

  sensor_t sensor;
  sensors_event_t event;

  command = readData();

  switch (command) {
    case 0 :
      //set digital low
      digitalWrite(readData(), LOW); break;
    case 1 :
      //set digital high
      digitalWrite(readData(), HIGH); break;
    case 2 :
      //get digital value
      Serial.println(digitalRead(readData())); break;
    case 3 :
      // set analog value
      analogWrite(readData(), readData()); break;
    case 4 :
      //read analog value
      Serial.println(analogRead(readData())); break;
    case 11 :
      //read temperature value
      Alarm.delay(delayMS);
      dht.temperature().getEvent(&event);
      Serial.println(event.temperature); break;
    case 15 :
      //read humidity value
      Alarm.delay(delayMS);
      dht.humidity().getEvent(&event);
      Serial.println(event.relative_humidity); break;
    case 10 :
      //read sensor delay
      dht.temperature().getSensor(&sensor);
      Serial.println(delayMS); break;
    case 12 :
      //read sensor t max
      dht.temperature().getSensor(&sensor);
      Serial.println(sensor.max_value); break;
    case 13 :
      //read sensor t min
      dht.temperature().getSensor(&sensor);
      Serial.println(sensor.min_value); break;
    case 14 :
      //read sensor t res
      dht.temperature().getSensor(&sensor);
      Serial.println(sensor.resolution); break;
    case 16 :
      //read sensor h max
      dht.humidity().getSensor(&sensor);
      Serial.println(sensor.max_value); break;
    case 17 :
      //read sensor h min
      dht.humidity().getSensor(&sensor);
      Serial.println(sensor.min_value); break;
    case 18 :
      //read sensor h res
      dht.humidity().getSensor(&sensor);
      Serial.println(sensor.resolution); break;
    /*
      case 20 :
      //read sensor lux
      if (BH1750_Read(BH1750_address) == 2) {
       valLx = ((buff[0] << 8) | buff[1]) / 1.2;
       if (valLx < 0)Serial.print("> 65535");
       else {
         Serial.print((int)valLx, DEC);
       }
       Serial.print(" lx");
       Serial.print("/t");
      } break;
    */
    case 30 :
      // read moisture sensure value
      temp[1] = readData();
      temp[2] = readData();
      Serial.println(greeneyMoistSens(temp[1], temp[2])); break;
    case 50 :
      //read day hours setting
      Serial.println(d); break;
    case 51 :
      //read night hours setting
      Serial.println(n); break;
    case 52 :
      //set day hours
      d = readData();
      interval1 = (60 * d); break; // interval1 = (3600 * d); break;
    case 53 :
      //set night hours
      n = readData();
      interval2 = (60 * n); break; // interval2 = (3600 * n); break;
    case 54 :
      //read time setting
      Serial.print(hour()); Serial.print("\t"); Serial.print(minute()); Serial.print("\t"); Serial.print(second());
      Serial.print("\t"); Serial.print(day()); Serial.print("\t"); Serial.print(month()); Serial.print("\t"); Serial.println(year()); break;
    case 55:
      //set time
      /*
        for (int i=1; i <= 6; i++){
          temp[i] = readData();
        }
      */
      //longTemp[1] = now();
      temp[1] = readData();
      temp[2] = readData();
      temp[3] = readData();
      temp[4] = readData();
      temp[5] = readData();
      temp[6] = readData();
      greeneySetTime(temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]); break;

    case 56 :
      // read the total alive time in seconds
      alive = (now() - timeOffset);
      Serial.println(alive); break;
    case 60 :
      // read the light status
      Serial.println(lightOn); break;
    case 61 :
      //turn the lights on
      greeneyLightsOn(); break;
    case 62 :
      //turn the lights off
      greeneyLightsOff(); break;
    
    case 80 :
      // set whether load settings from EEPROM on start
      temp[0] = readData();
      EEPROM.write(2, temp[0]); break;

    case 81 :
      // read the day hours value from EEEPROM
      Serial.println(EEPROM.read(0)); break;

    case 82 :
      // read the night hours value from EEEPROM
      Serial.println(EEPROM.read(1)); break;

    case 83 :
      // write the current day hours value to EEEPROM
      EEPROM.write(0, d); break;

    case 84 :
      // write the current night hours value to EEEPROM
      EEPROM.write(1, n); break;

    case 85 :
      // set the day hours from the EEEPROM
      d =  EEPROM.read(0); break;

    case 86 :
      // set the night hours from the EEEPROM
      n = EEPROM.read(1); break;

    case 87 :
      //clear EEEPROM      
      for (int i = 0 ; i < 7 ; i++) {
      //for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
      } break;

    case 88 :
      //prints the first X digits of the EEPROM
      temp[0] = readData();
      if (temp[0] < EEPROM.length()) {
         for (int i = 0 ; i < (temp[0]-1) ; i++) {
            Serial.print(EEPROM.read(i));
            Serial.print("\t");
          }
          Serial.println();
      } break;

      case 89 :
        //write arbitrary value at arbitrary EEPROM address
        //CAUTION!!
        temp[0] = readData();
        temp[1] = readData();
        EEPROM.write(temp[0], temp[1]); break;

    case 70 :
      // read the automation status
      if (automation) {
        Serial.println("1");
      }
      else {
        Serial.println("0"); 
      } break;

    case 71 :
      // turn both timers on
       greeneyAutomationOn(); break;
      
    case 72 :
      // turn both timers off
      greeneyAutomationOff(); break;
      
    case 99:
      //just dummy to cancel the current read, needed to prevent lock
      //when the PC side dropped the "w" that we sent
      break;

  }
}



/*
  void BH1750_Init(int address) {

  Wire.beginTransmission(address);
  Wire.write(0x10); // 1 [lux] resolution
  Wire.endTransmission();
  }

  byte BH1750_Read(int address) {

  byte i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available()) {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
  }
*/



