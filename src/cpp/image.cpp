#include "../hpp/image.hpp"

DeltaEImage::DeltaEImage(std::string image_path){

    int width, height, channels;
    
    // The width is measured in PIXELS, not in BYTES!
    unsigned char* raw_data = stbi_load(image_path.c_str(),
                                    &width, &height,
                                    &channels, this->NUM_CHANNELS);

    size_t total_bytes = width * height * this->NUM_CHANNELS;                             

    if(not raw_data){
        throw std::runtime_error("Failed to fetch image data from: " + image_path);
    }

    std::vector<uint8_t> data(raw_data, raw_data + total_bytes);

    this->set_pixels(data, width, height);
    this->width = width;
    this->height = height;

    stbi_image_free(raw_data);
}

DeltaEImage::DeltaEImage(std::vector<uint8_t> image_data, int width, int height){
    this->width = width;
    this->height = height;
    this->set_pixels(image_data, width, height);
}

void DeltaEImage::set_pixels(const std::vector<uint8_t>& image_data, int width, int height){

    int num_pixels = width * height;
    int num_bytes = num_pixels * this->NUM_CHANNELS;
    this->pixels.reserve(num_pixels);
    
    int byte_index, pixel_index;

    for(byte_index = 0, pixel_index = 0;
        byte_index < num_bytes;
        byte_index+=this->NUM_CHANNELS, pixel_index++){

        uint8_t red = image_data[byte_index + 0];
        uint8_t green = image_data[byte_index + 1];
        uint8_t blue = image_data[byte_index + 2];

        this->pixels.emplace_back(DeltaEPixel(red, green, blue));
    }
}

std::vector<uint8_t> DeltaEImage::get_data(){

    std::vector<uint8_t> data;
    int num_bytes = this->pixels.size() * this->NUM_CHANNELS;
    data.resize(num_bytes);

    int pixel_index, byte_index;

    for(pixel_index = 0, byte_index = 0;
        pixel_index < this->pixels.size();
        pixel_index++, byte_index+=this->NUM_CHANNELS){

        data[byte_index+0] = this->pixels.at(pixel_index).get_red_channel();
        data[byte_index+1] = this->pixels.at(pixel_index).get_green_channel();
        data[byte_index+2] = this->pixels.at(pixel_index).get_blue_channel();
    }

    return data;
}

std::vector<DeltaEPixel> DeltaEImage::get_pixels(){
    return this->pixels;
}

DeltaEPixel DeltaEImage::get_pixel_at(unsigned int x, unsigned int y){
    if(x >= this->width or x < 0){
        throw std::length_error("'x' coordinate is out of range.");
    }
    if(y >= this->height or y < 0){
        throw std::length_error("'y' coordinate is out of range.");
    }
    int linearized_index = this->get_linearized_index(x, y);
    return this->pixels.at(linearized_index);
}

int DeltaEImage::get_width(){
    return this->width;
}

int DeltaEImage::get_height(){
    return this->height;
}

void DeltaEImage::save(std::string path){
    int bytes_per_row = this->width * this->NUM_CHANNELS;

    bool result = stbi_write_png(path.c_str(),
                                 this->width,
                                 this->height,
                                 this->NUM_CHANNELS,
                                 this->get_data().data(),
                                 bytes_per_row);
    if(not result){
        throw std::runtime_error("Failed to save image in path: " + path);
    }
}

int DeltaEImage::get_linearized_index(unsigned int x, unsigned int y){
    return (y * this->width) + x;
}

void DeltaEImage::set_pixel_at(unsigned int x, unsigned int y, DeltaEPixel pixel){
    int linearized_index = this->get_linearized_index(x, y);
    this->pixels[linearized_index] = pixel;
}