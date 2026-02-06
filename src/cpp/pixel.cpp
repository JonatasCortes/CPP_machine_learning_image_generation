#include "../hpp/pixel.hpp"

RgbPixel::RgbPixel(uint8_t red, uint8_t green, uint8_t blue){
  this->red_channel = red;
  this->green_channel = green;
  this->blue_channel = blue;
}