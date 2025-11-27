#include "Display.cpp"

Display *display;

// Time tracking variables
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL = 60000; // Update every minute
const unsigned long DISPLAY_INTERVAL = 3600000; // Display time every hour

int currentHour = 12;   // Starting time (adjust as needed)
int currentMinute = 0;

void setup()
{
  Serial.begin(115200);
  display = new Display();
  
  Serial.println("Haptic Clock Started");
  Serial.println("Time will be displayed every hour");
  
  // Display initial time
  display->displayTime(currentHour, currentMinute);
  lastUpdateTime = millis();
}

void updateTime()
{
  currentMinute++;
  
  if (currentMinute >= 60) {
    currentMinute = 0;
    currentHour++;
    
    if (currentHour >= 24) {
      currentHour = 0;
    }
  }
}

void loop()
{
  unsigned long currentTime = millis();
  
  // Update time every minute
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    updateTime();
    lastUpdateTime = currentTime;
    
    // Display time every hour (when minutes == 0)
    if (currentMinute == 0) {
      Serial.println("\n=== Hour Change ===");
      display->displayTime(currentHour, currentMinute);
    }
  }
  
  // Optional: Check for serial commands to manually trigger time display
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 't' || cmd == 'T') {
      Serial.println("\n=== Manual Time Display ===");
      display->displayTime(currentHour, currentMinute);
    }
  }
}