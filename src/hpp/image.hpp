#ifndef IMAGE_HPP

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#include "pixel.hpp"
#include "../../include/stb_image.h"
#include "../../include/stb_image_write.h"


class DeltaEImage{

    private:

        int width;
        int height;
        const int NUM_CHANNELS = 3;

        std::vector<DeltaEPixel> pixels;
        void set_pixels(const std::vector<uint8_t>& image_data, int width, int height);

    public:

        DeltaEImage(std::string image_path);
        DeltaEImage(std::vector<uint8_t> image_data ,int width, int height);

        std::vector<uint8_t> get_data();
        std::vector<DeltaEPixel> get_pixels();
        DeltaEPixel get_pixel_at(unsigned int x, unsigned int y);

        int get_height();
        int get_width();

        void save(std::string path);
};

#endif