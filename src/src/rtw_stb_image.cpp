#define STB_IMAGE_IMPLEMENTATION
#include "lib/rtw_stb_image.h"

rtw_image::rtw_image() {}

rtw_image::rtw_image(const char* image_filename) {
    int components_per_pixel = 3;
    m_data = stbi_load(image_filename, &m_width, &m_height, &m_bytes_per_pixel, components_per_pixel);

    if (!m_data) {
        std::cerr << "ERROR: Could not load texture image file '" << image_filename << "'.\n";
        std::cerr << "Reason: " << stbi_failure_reason() << "\n";
        m_width = m_height = 0;
        m_bytes_per_pixel = 0;
    } else {
        m_bytes_per_pixel = components_per_pixel;
    }
}

rtw_image::~rtw_image() {
    stbi_image_free(m_data);
}

const unsigned char* rtw_image::pixel_data(int x, int y) const {
    if (!m_data || x < 0 || x >= m_width || y < 0 || y >= m_height) return nullptr;
    int index = y * m_width * m_bytes_per_pixel + x * m_bytes_per_pixel;
    return &m_data[index];
}
