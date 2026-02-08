#include <cstdint>
#include <array>
#include <cmath>
#include <ranges>

using namespace std;

class DeltaEPixel {

    private:

        float cie_lab_L;
        float cie_lab_A;
        float cie_lab_B;

        uint8_t red_channel;
        uint8_t green_channel;
        uint8_t blue_channel;

        std::array<float, 3> convert_CIE_XYZ_into_CIE_LAB(array<float, 3> cie_xyz);
        std::array<float, 3> normalize_CIE_XYZ_by_relative_white(array<float, 3> cie_xyz);
        std::array<float, 3> correct_CIE_XYZ_by_light(array<float, 3> normalized_cie_xyz);
        float non_linear_CIE_LAB_lightness_correction(float normalized_cie_xyz_component);
        
        std::array<float, 3> convert_linear_rgb_into_CIE_XYZ(array<float, 3> linear_rgb);
        std::array<float, 3> linearize_rgb_values(uint8_t red, uint8_t green, uint8_t blue);
        float linearize_rgb_channel(double normalized_channel);

    public:

        DeltaEPixel(uint8_t red, uint8_t green, uint8_t blue);

        inline uint8_t get_red_channel(){return this->red_channel;};
        inline uint8_t get_green_channel(){return this->green_channel;};
        inline uint8_t get_blue_channel(){return this->blue_channel;};

        inline float get_cie_lab_L(){return this->cie_lab_L;};
        inline float get_cie_lab_A(){return this->cie_lab_A;};
        inline float get_cie_lab_B(){return this->cie_lab_B;};
    };