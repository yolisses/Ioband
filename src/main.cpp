#include "Display.cpp"

Display *display;

void setup()
{
  display = new Display();
}

void loop()
{
  for (auto i = 0; i < 10; i++)
  {
    display->displayDigit(i);
  }
}
