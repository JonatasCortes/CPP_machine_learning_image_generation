#ifndef IMAGE_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#include "pixel.hpp"
#include "../../include/stb_image.h"
#include "../../include/stb_image_write.h"


class RgbImage{

    private:

    int width;
    int height;

    std::vector<std::vector<RgbPixel>> pixels;
    void set_pixels(std::vector<uint8_t> image_data, int width, int height, int num_chanels = 3);

    public:

    RgbImage(std::string image_path);
    RgbImage(std::vector<uint8_t> image_data ,int width, int height);

    std::vector<uint8_t> get_data();
    std::vector<std::vector<RgbPixel>> get_pixels();
    RgbPixel get_pixel_at(unsigned int x, unsigned int y);

    int get_height();
    int get_width();

    void save(std::string path);
};

#endif