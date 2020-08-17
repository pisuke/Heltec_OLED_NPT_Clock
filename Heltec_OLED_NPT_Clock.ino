/*
  ESP32 NTP clock

  Used libraries:
  ESP32WiFi library
  Time library https://github.com/PaulStoffregen/Time
  ESPPerfectTime libary https://github.com/hunamizawa/ESPPerfectTime
  Heltec ESP32 kit library
*/

#include "Arduino.h"
#include "heltec.h"
#include <ESPPerfectTime.h>
#include <TimeLib.h> // Time library https://github.com/PaulStoffregen/Time
#include <WiFi.h>
//#include <WiFiUdp.h>
#include <ArduinoJson.h>

// WiFi configuration
const char ssid[] = "bozzograo";  //  your network SSID (name)
const char pass[] = "E398f98c22";       // your network password

// NTP configuration
const char *ntpServerName = "pool.ntp.org";
const int timeZone = 0;     // UTC
const int daylightOffset_sec = 3600; // DST offset in seconds

// Other variables and objects
WiFiClient wifiClient;

char timeZoneAbbreviation[10];
char timeZone_str[40];
time_t prevDisplay = 0; // when the digital clock was displayed

void setup()
{
  // Setup OLED Display
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->flipScreenVertically();

  // Initialise the Serial Interface
  Serial.begin(115200);
  while (!Serial) ; // Needed for Leonardo only
  delay(250);
  Serial.println("\nESP32 NTP clock");

  // Connect to WiFi SSID
  connectWiFi();

  // Request http://worldtimeapi.org/api/ip
  DynamicJsonDocument jsonDoc = httpJSONRequest(wifiClient, "worldtimeapi.org", "/api/ip");

  strcpy(timeZoneAbbreviation, jsonDoc["abbreviation"]);
  //  const char* client_ip = jsonDoc["client_ip"]; // "86.17.220.24"
  //  const char* dateTime = jsonDoc["datetime"]; // "2020-07-25T14:51:01.689928+01:00"
  //  int day_of_week = jsonDoc["day_of_week"]; // 6
  //  int day_of_year = jsonDoc["day_of_year"]; // 207
  bool dst = jsonDoc["dst"]; // true
  //  const char* dst_from = jsonDoc["dst_from"]; // "2020-03-29T01:00:00+00:00"
  int dst_offset = jsonDoc["dst_offset"]; // 3600
  //  const char* dst_until = jsonDoc["dst_until"]; // "2020-10-25T01:00:00+00:00"
  //  int raw_offset = jsonDoc["raw_offset"]; // 0
  strcpy(timeZone_str, jsonDoc["timezone"]);
  //  long unixtime = jsonDoc["unixtime"]; // 1595685061
  //  const char* utc_datetime = jsonDoc["utc_datetime"]; // "2020-07-25T13:51:01.689928+00:00"
  const char* utc_offset = jsonDoc["utc_offset"]; // "+01:00"
  //  int week_number = jsonDoc["week_number"]; // 30

  // convert UTC offset to signed float
  float utc_offset_float = ((float)utc_offset[1] - 48) * 10 + ((float)utc_offset[2] - 48) + (((float)utc_offset[4] - 48) * 10 + ((float)utc_offset[5] - 48)) / 60.0;
  if ((int)utc_offset[0] == 45)  {
    utc_offset_float *= -1;
  }

  Serial.println(timeZoneAbbreviation);
  //  Serial.println(client_ip);
  //  Serial.println(dateTime);
  //  Serial.println(day_of_week);
  //  Serial.println(day_of_year);
  Serial.println(dst);
  //  Serial.println(dst_from);
  //  Serial.println(dst_offset);
  //  Serial.println(dst_until);
  //  Serial.println(raw_offset);
  Serial.println(timeZone_str);
  //  Serial.println(unixtime);
  //  Serial.println(utc_datetime);
  Serial.println(utc_offset);
  Serial.println(utc_offset_float);
  //  Serial.println(week_number);

  //pftime::configTime(utc_offset_float * 3600, dst_offset*dst, ntpServerName);
  pftime::configTime(utc_offset_float * 3600, 0, ntpServerName);
}


void loop()
{

  if (now() != prevDisplay) { //update the display only if time has changed
    prevDisplay = now();
    // Get current local time as struct tm by calling pftime::localtime(nullptr)
    struct tm *tm = pftime::localtime(nullptr);
    // Get microseconds at the same time by passing suseconds_t* as 2nd argument
    suseconds_t usec;
    tm = pftime::localtime(nullptr, &usec);

    // Get the time string
    String dateString = getDateString(tm, usec);
    String timeString = getTimeString(tm, usec);

    // Print the time to serial
    Serial.println(dateString + " | " + timeString + " " + timeZoneAbbreviation + " " + timeZone_str);

    Heltec.display->clear();
    OLEDClockDisplay(64, 0, ArialMT_Plain_16, TEXT_ALIGN_CENTER, dateString);
    OLEDClockDisplay(0, 18, ArialMT_Plain_24, TEXT_ALIGN_LEFT, timeString);
    OLEDClockDisplay(128, 25, ArialMT_Plain_10, TEXT_ALIGN_RIGHT, timeZoneAbbreviation);
    OLEDClockDisplay(64, 43, ArialMT_Plain_10, TEXT_ALIGN_CENTER, timeZone_str);
    OLEDClockDisplay(64, 54, ArialMT_Plain_10, TEXT_ALIGN_CENTER,  WiFi.localIP().toString());
    Heltec.display->display();
  }
}

void OLEDClockDisplay(int x, int y, const uint8_t* font, OLEDDISPLAY_TEXT_ALIGNMENT alignment, String targetString)
{
  Heltec.display->setTextAlignment(alignment);
  Heltec.display->setFont(font);
  Heltec.display->drawString(x, y, targetString);
}


void connectWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, pass);
  Serial.print("\nConnecting to WiFi SSID " + String(ssid) + " ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected. ");
  Serial.print("The IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
}

String getDateString(struct tm *tm, suseconds_t usec) {
  char time_string[100];
  sprintf(time_string, "%04d / %02d / %02d",
          tm->tm_year + 1900,
          tm->tm_mon + 1,
          tm->tm_mday);
  return time_string;
}


String getTimeString(struct tm *tm, suseconds_t usec) {
  char time_string[100];
  sprintf(time_string, "%02d:%02d:%02d",
          tm->tm_hour,
          tm->tm_min,
          tm->tm_sec);
  return time_string;
}
