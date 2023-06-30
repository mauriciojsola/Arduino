#include <EEPROM.h>
#include <ArduinoJson.h>

#define EEPROM_SIZE 512

int metaAddress = 0;
int metaLenght = 4;
int jsonAddress = 4;

String EEPROM_read(int index, int length) {
  String text = "";
  char ch = 1;

  for (int i = index; (i < (index + length)) && ch; ++i) {
    if (ch = EEPROM.read(i)) {
      text.concat(ch);
    }
  }
  return text;
}

int EEPROM_write(int index, String text) {
  for (int i = index; i < text.length() + index; ++i) {
    EEPROM.write(i, text[i - index]);
  }
  EEPROM.write(index + text.length(), 0);
  EEPROM.commit();

  return text.length() + 1;
}

DynamicJsonDocument getEEPROM_JSON() {

  String jsonRead = EEPROM_read(jsonAddress,
                                EEPROM_read(metaAddress, metaLenght).toInt());

  // Serial.print("JSON Read: ");
  // Serial.println(jsonRead);

  DynamicJsonDocument jsonDoc(EEPROM_SIZE);

  deserializeJson(jsonDoc, jsonRead);

  return jsonDoc;
}

void setEEPROM_JSON(DynamicJsonDocument jsonDoc) {

  String jsonWriteString;

  serializeJson(jsonDoc, jsonWriteString);

  // Serial.print("JSON Write: ");
  // Serial.println(jsonWriteString);

  EEPROM_write(metaAddress,
               (String)EEPROM_write(jsonAddress, jsonWriteString));
}

void setup() {

  Serial.begin(9600);

  delay(1000);

  EEPROM.begin(EEPROM_SIZE);

  DynamicJsonDocument jsonDoc(EEPROM_SIZE);

  jsonDoc["sensor"] = "gps";
  jsonDoc["time"] = 1351824120;
  jsonDoc["data"][0] = 48.756080;
  jsonDoc["data"][1] = 2.302038;

  setEEPROM_JSON(jsonDoc);
}

long delayTime = 10000;

void loop() {

  DynamicJsonDocument jsonDoc = getEEPROM_JSON();

  const char *sensor = jsonDoc["sensor"];
  long time = jsonDoc["time"];
  double latitude = jsonDoc["data"][0];
  double longitude = jsonDoc["data"][1];

  Serial.print("sensor: ");
  Serial.println(sensor);
  Serial.print("time: ");
  Serial.println(time);
  Serial.print("latitude: ");
  Serial.println(latitude);
  Serial.print("longitude: ");
  Serial.println(longitude);

  jsonDoc["time"] = time + delayTime;

  setEEPROM_JSON(jsonDoc);

  delay(delayTime);
}
