#include "../hpp/image.hpp"

RgbImage::RgbImage(std::string image_path){

    int width, height, chanels, num_chanels = 3;
    
    // The width is measured in PIXELS, not in BYTES!
    unsigned char* raw_data = stbi_load(image_path.c_str(),
                                    &width, &height,
                                    &chanels, num_chanels);

    size_t total_bytes = width * height * num_chanels;                             

    if(not raw_data){
        throw std::runtime_error("Failed to fetch image data from: " + image_path);
    }

    std::vector<uint8_t> data(raw_data, raw_data + total_bytes);

    this->set_pixels(data, width, height);
    this->width = width;
    this->height = height;

    stbi_image_free(raw_data);
}

RgbImage::RgbImage(std::vector<uint8_t> image_data, int width, int height){
    this->width = width;
    this->height = height;
    this->set_pixels(image_data, width, height);
}

void RgbImage::set_pixels(std::vector<uint8_t> image_data, int width, int height, int num_chanels){
    
    std::vector<std::vector<RgbPixel>> pixels(height);
    int width_in_bytes = width * num_chanels;
    int x_offset = 0;

    for(int y_index = 0; y_index < height; y_index++){
        for(int x_index = 0; x_index < width_in_bytes; x_index += num_chanels){
            uint8_t red = image_data[x_index + x_offset + 0];
            uint8_t green = image_data[x_index + x_offset + 1];
            uint8_t blue = image_data[x_index + x_offset + 2];
            pixels.at(y_index).push_back(RgbPixel(red, green, blue));

        }
        x_offset += width_in_bytes;
    }

    this->pixels = pixels;
}

std::vector<uint8_t> RgbImage::get_data(){

    std::vector<uint8_t> data;

    for(auto row: this->pixels){
        for(RgbPixel pixel: row){
            data.push_back(pixel.red_channel);
            data.push_back(pixel.green_channel);
            data.push_back(pixel.blue_channel);

        }
    }

    return data;
}

std::vector<std::vector<RgbPixel>> RgbImage::get_pixels(){
    return this->pixels;
}

RgbPixel RgbImage::get_pixel_at(unsigned int x, unsigned int y){
    if(x >= this->width or x < 0){
        throw std::length_error("'x' coordinate is out of range.");
    }
    if(y >= this->height or y < 0){
        throw std::length_error("'y' coordinate is out of range.");
    }

    return this->pixels.at(y).at(x);
}

int RgbImage::get_width(){
    return this->width;
}

int RgbImage::get_height(){
    return this->height;
}

void RgbImage::save(std::string path){
    int num_chanels = 3;
    int bytes_per_row = this->width * num_chanels;
    std::vector<uint8_t> data = this->get_data();
    bool result = stbi_write_png(path.c_str(),
                                 this->width,
                                 this->height,
                                 num_chanels,
                                 data.data(),
                                 bytes_per_row);
    if(not result){
        throw std::runtime_error("Failed to save image in path: " + path);
    }
}