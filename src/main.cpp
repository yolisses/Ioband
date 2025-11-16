#include "Display.cpp"

Display *display;

void setup()
{
  Serial.begin(115200);
  display = new Display();
}

void loop()
{
  display->displayEncoding({1, 0, 0, 0, 0}, 1000);
  display->displayEncoding({0, 1, 0, 0, 0}, 1000);
  display->displayEncoding({0, 0, 1, 0, 0}, 1000);
  display->displayEncoding({0, 0, 0, 1, 0}, 1000);
  display->displayEncoding({0, 0, 0, 0, 1}, 1000);
}
