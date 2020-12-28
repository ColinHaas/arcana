#ifndef __GRAPHICS_H___
#define __GRAPHICS_H___

#include <Particle.h>

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white = 0)
        : red(red), green(green), blue(blue), white(white) {}

    Color operator+(const Color &c) const
    {
        return Color(uint8_t(min(uint16_t(red) + c.red, 255)),
                     uint8_t(min(uint16_t(green) + c.green, 255)),
                     uint8_t(min(uint16_t(blue) + c.blue, 255)),
                     uint8_t(min(uint16_t(white) + c.white, 255)));
    }
};

#endif
