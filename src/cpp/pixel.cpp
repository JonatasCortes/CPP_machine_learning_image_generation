#include "../hpp/pixel.hpp"


DeltaEPixel::DeltaEPixel(uint8_t red, uint8_t green, uint8_t blue){

    this->red_channel = red;
    this->blue_channel = blue;
    this->green_channel = green;

    array<float, 3> linear_rgb = this->linearize_rgb_values(red, green, blue);
    array<float, 3> cie_xyz = this->convert_linear_rgb_into_CIE_XYZ(linear_rgb);
    array<float, 3> cie_lab = this->convert_CIE_XYZ_into_CIE_LAB(cie_xyz);

    this->cie_lab_L = cie_lab.at(0);
    this->cie_lab_A = cie_lab.at(1);
    this->cie_lab_B = cie_lab.at(2);
}


array<float, 3> DeltaEPixel::convert_CIE_XYZ_into_CIE_LAB(array<float, 3> cie_xyz){

    array<float, 3> cie_xyz_normalized = this->normalize_CIE_XYZ_by_relative_white(cie_xyz);
    array<float, 3> cie_xyz_corrected = this->correct_CIE_XYZ_by_light(cie_xyz_normalized);

    array<float, 3> cie_lab;

    cie_lab[0] = 116 * cie_xyz_corrected.at(1) - 16;
    cie_lab[1] = 500 * (cie_xyz_corrected.at(0) - cie_xyz_corrected.at(1));
    cie_lab[2] = 200 * (cie_xyz_corrected.at(1) - cie_xyz_corrected.at(2));

    return cie_lab;
}


array<float, 3> DeltaEPixel::normalize_CIE_XYZ_by_relative_white(array<float, 3> cie_xyz){

    array<float, 3> cie_xyz_normalized_by_relative_white;
    constexpr array<double, 3> relative_white = {0.95047, 1.00000, 1.08883};

    for(int index = 0; index < cie_xyz.size(); index++){
        double normalized_component = cie_xyz.at(index) / relative_white.at(index);
        cie_xyz_normalized_by_relative_white[index] = static_cast<float>(normalized_component);
    }

    return cie_xyz_normalized_by_relative_white;
}

array<float, 3> DeltaEPixel::correct_CIE_XYZ_by_light(array<float, 3> normalized_cie_xyz){

    array<float, 3> cie_xyz_corrected_by_light;

    int index = 0;
    for(float component: normalized_cie_xyz){
        float corrected_component = this->non_linear_CIE_LAB_lightness_correction(component);
        cie_xyz_corrected_by_light[index] = corrected_component;
        index++;
    }

    return cie_xyz_corrected_by_light;
}


float DeltaEPixel::non_linear_CIE_LAB_lightness_correction(float normalized_cie_xyz_component){

    double component = static_cast<double>(normalized_cie_xyz_component);

    constexpr double delta = 6.0 / 29.0;
    constexpr double squared_delta = delta * delta;
    constexpr double cubic_delta = squared_delta * delta;

    if(component > cubic_delta)
        return static_cast<float>(pow(component, 1.0/3.0));

    return static_cast<float>((component / (3 * squared_delta)) + (4.0/29.0));
}


array<float, 3> DeltaEPixel::convert_linear_rgb_into_CIE_XYZ(array<float, 3> linear_rgb){

    double red = static_cast<double>(linear_rgb.at(0)),
           green = static_cast<double>(linear_rgb.at(1)),
           blue = static_cast<double>(linear_rgb.at(2));

    constexpr array<double, 3> x_wheights = {0.4124564, 0.3575761, 0.1804375};
    constexpr array<double, 3> y_wheights = {0.2126729, 0.7151522, 0.0721750};
    constexpr array<double, 3> z_wheights = {0.0193339, 0.1191920, 0.9503041};

    constexpr array<array<double, 3>, 3> weights = {x_wheights, y_wheights, z_wheights};

    array<float, 3> cie_xyz;

    for(int index = 0; index < cie_xyz.size(); index++){
        cie_xyz[index] = static_cast<float>(weights.at(index).at(0) * red +
                                            weights.at(index).at(1) * green +
                                            weights.at(index).at(2) * blue);        
    }

    return cie_xyz;
}


array<float, 3> DeltaEPixel::linearize_rgb_values(uint8_t red, uint8_t green, uint8_t blue){

    array<double, 3> normalized_rgb = {red/255.0,
                                       green/255.0,
                                       blue/255.0};
    array<float, 3> linearized_rgb;

    int index = 0;
    for(double channel: normalized_rgb){
        linearized_rgb[index] = this->linearize_rgb_channel(channel);
        index++;        
    }

    return linearized_rgb;
}


float DeltaEPixel::linearize_rgb_channel(double normalized_channel){

    if(normalized_channel <= 0.04045)
        return static_cast<float>(normalized_channel/12.92);

    return static_cast<float>(pow((normalized_channel + 0.055)/1.055, 2.4));
}