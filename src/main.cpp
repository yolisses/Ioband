#include "Display.cpp"

Display *display;

void setup()
{
  Serial.begin(115200);
  display = new Display();
}

void loop()
{
  for (auto i = 0; i < 10; i++)
  {
    display->displayDigit(i);
  }
}
