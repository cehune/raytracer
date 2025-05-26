#include "texture.h"
#include <cmath>  // for std::floor

// solid_color

solid_color::solid_color(const color& albedo) : albedo(albedo) {}

solid_color::solid_color(double red, double green, double blue)
    : solid_color(color(red, green, blue)) {}

color solid_color::value(double u, double v, const vec3h& p) const {
    return albedo;
}

// checker_texture 

checker_texture::checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
    : inv_scale(1.0 / scale), even(even), odd(odd) {}

checker_texture::checker_texture(double scale, const color& c1, const color& c2)
    : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

color checker_texture::value(double u, double v, const vec3h& p) const {
    int xInteger = int(std::floor(inv_scale * p.x));
    int yInteger = int(std::floor(inv_scale * p.y));
    int zInteger = int(std::floor(inv_scale * p.z));
    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
}

// image_texture

image_texture::image_texture(const char* filename) : image(filename) {}

color image_texture::value(double u, double v, const vec3h& p) const {
    if (image.height() <= 0) return color(0, 1, 1); // fallback cyan

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0, 1);
    v = 1.0 - clamp(v, 0, 1);

    int i = int(u * image.width());
    int j = int(v * image.height());
    auto pixel = image.pixel_data(i, j);

    const double color_scale = 1.0 / 255.0;
    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}
