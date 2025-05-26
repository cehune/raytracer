// rtw_stb_image.h
#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

// ❌ REMOVE THIS LINE:
// #define STB_IMAGE_IMPLEMENTATION

#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <cstdlib>
#include <iostream>

class rtw_image {
public:
    rtw_image();
    rtw_image(const char* image_filename);
    ~rtw_image();

    int width() const { return m_width; }
    int height() const { return m_height; }

    const unsigned char* pixel_data(int x, int y) const;

private:
    unsigned char* m_data = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_bytes_per_pixel = 0;
};

#endif // RTW_STB_IMAGE_H
