#ifndef PIXEL_HPP
#include <cstdint>


class RgbPixel{

    public:

        uint8_t red_channel;
        uint8_t blue_channel;
        uint8_t green_channel;

        RgbPixel(uint8_t red, uint8_t green, uint8_t blue);
};

#endif