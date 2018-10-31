#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password =  "";

const String endpoint = "http://api.openweathermap.org/data/2.5/forecast?id=472757&cnt=8&units=metric&APPID=";
const String key = "";

int timeUTC[40];
float temp[40];
float rain[40];
float cloud[40];
float snow[40];

int morningNumber = 99;
int eveningNumber = 99;

struct {
  uint32_t crc32;
  byte data[508];
} rtcData;

int morningHour = 7;
int eveningHour = 18;

String getWeatherData() {
  
  String payload = "";
  HTTPClient http;

  http.begin(endpoint + key);
  int httpCode = http.GET();

  if (httpCode == 200) {

    payload = http.getString();
  }

  else {
    payload = "E";

  }
  
  http.end();

  int i = 0;
  int last = payload.indexOf(",{""dt", 0);
  while (i < 8) {
    last = payload.indexOf(",{", last + 1);
    i = i + 1;
  }

  return payload.substring(0, last) + "]}";
  
}

void parsePaket(int number, JsonObject& liste) {

  temp[number] = liste["main"]["temp"]; // 20.31
  Serial.print(" Temp ");
  Serial.print(temp[number]);

  rain[number] = liste["rain"]["3h"]; // 0
  Serial.print(" Rain ");
  Serial.print(rain[number]);

  snow[number] = liste["snow"]["3h"]; // 0
  Serial.print(" Snow ");
  Serial.print(snow[number]);

  const char* list_dt_txt = liste["dt_txt"]; // "2018-10-17 15:00:00"
  timeUTC[number] = (list_dt_txt[11] - '0') * 10 + list_dt_txt[12] - '0';
  Serial.print(" Time ");
  Serial.println(timeUTC[number]);

}

void decodeWeather(String JSONline) {

  const size_t bufferSize = 4 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(4) + 8 * JSON_OBJECT_SIZE(1) + 4 * JSON_OBJECT_SIZE(2) + 5 * JSON_OBJECT_SIZE(4) + 4 * JSON_OBJECT_SIZE(7) + 4 * JSON_OBJECT_SIZE(8) + 1270;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  const char* json = JSONline.c_str();
  JsonObject& root = jsonBuffer.parseObject(json);
  JsonArray& list = root["list"];
  for (int i = 0; i < 8; i++)parsePaket(i, list[i]);
  
}

void showData(int dayTime, int index) {

  // Rain
  Serial.print(", Rain ");
  Serial.print(rain[index]);

  // Snow
  Serial.print(", Snow ");
  Serial.println(snow[index]);

}

void setup() {
  
  String weather;

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    do {
      weather = getWeatherData();
      Serial.println();
      
      //Serial.println(weather); //Show response

    }  while (weather[0] == 'E');
    decodeWeather(weather);

    for (int i = 0; i < 8 ; i++) {
      if (timeUTC[i] > (morningHour) && timeUTC[i] <= morningHour + 3) morningNumber = i;
      if (timeUTC[i] > (eveningHour) && timeUTC[i] <= eveningHour + 3) eveningNumber = i;
    }
    
    Serial.println();
    
    Serial.print("Morning Temperature: ");
    Serial.print(morningNumber);
    showData(0, morningNumber);
    Serial.print("Evening Temperature: ");
    Serial.print(eveningNumber);
    showData(1, eveningNumber);
    
    Serial.println();

  }

  Serial.print("Time spent: ");
  Serial.println(millis());
  Serial.println("-----");

}

void loop() {
  
  yield();
  
}
