#include <esp32-hal-rgb-led.h>

void turnLEDWhite(unsigned int pin)
{
    digitalWrite(pin, HIGH);
}

void turnLEDOff(unsigned int pin)
{
    digitalWrite(pin, LOW);
}

void turnLEDRed(unsigned int pin)
{
    neopixelWrite(pin, RGB_BRIGHTNESS, 0, 0);
}

void turnLEDGreen(unsigned int pin)
{
    neopixelWrite(pin, 0, RGB_BRIGHTNESS, 0);
}

void turnLEDBlue(unsigned int pin)
{
    neopixelWrite(pin, 0, 0, RGB_BRIGHTNESS);
}
