#include "Display.cpp"
#include "secrets.h"
#include <WiFi.h>
#include <time.h>
#include <esp_sleep.h>

Display *display;

// WiFi credentials from secrets.h
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// NTP Server settings
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = GMT_OFFSET;
const int daylightOffset_sec = DAYLIGHT_OFFSET;

// RTC memory to persist across deep sleep
RTC_DATA_ATTR bool timeInitialized = false;
RTC_DATA_ATTR unsigned long lastNTPSync = 0;

void connectWiFi()
{

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    delay(500);
    attempts++;
  }
}

void syncTimeWithNTP()
{

  connectWiFi();

  if (WiFi.status() == WL_CONNECTED)
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Wait for time to be set
    struct tm timeinfo;
    int attempts = 0;
    while (!getLocalTime(&timeinfo) && attempts < 10)
    {

      delay(500);
      attempts++;
    }

    if (attempts < 10)
    {

      timeInitialized = true;
      lastNTPSync = millis();

      // Display current time
      display->displayTime(timeinfo.tm_hour, timeinfo.tm_min);
    }

    // Disconnect WiFi to save power
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }
}

uint64_t calculateSleepTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // If time not available, sleep for 1 minute
    return 60ULL * 1000000ULL; // 1 minute in microseconds
  }

  // Calculate seconds until next minute
  int secondsUntilNextMinute = 60 - timeinfo.tm_sec;

  // Convert to microseconds
  return (uint64_t)secondsUntilNextMinute * 1000000ULL;
}

void setup()
{
  delay(1000);

  display = new Display();

  // Print wakeup reason
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  // Check if we need to sync time (once per day or first boot)
  unsigned long currentMillis = millis();
  bool needsSync = false;

  if (!timeInitialized)
  {
    needsSync = true;
  }
  else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
  {
    // Check if 24 hours have passed since last sync
    // Note: millis() resets on deep sleep, so we use RTC memory
    if (lastNTPSync > 86400000)
    { // More than 24 hours in milliseconds

      needsSync = true;
      lastNTPSync = 0;
    }
    else
    {
      lastNTPSync += currentMillis;
    }
  }

  if (needsSync)
  {
    syncTimeWithNTP();
  }
  else
  {
    // Just display the current time
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {

      display->displayTime(timeinfo.tm_hour, timeinfo.tm_min);
    }
  }

  // Calculate sleep time until next minute
  uint64_t sleepTime = calculateSleepTime();

  // Configure timer wakeup
  esp_sleep_enable_timer_wakeup(sleepTime);

  // Enter deep sleep
  esp_deep_sleep_start();
}

void loop()
{
  // We never reach here because of deep sleep
}