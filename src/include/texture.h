#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "lib/rtw_stb_image.h"

using std::shared_ptr;
using std::make_shared;

class texture {
public:
    virtual ~texture() = default;
    virtual color value(double u, double v, const vec3h& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(const color& albedo);
    solid_color(double red, double green, double blue);
    color value(double u, double v, const vec3h& p) const override;

private:
    color albedo;
};

class checker_texture : public texture {
public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd);
    checker_texture(double scale, const color& c1, const color& c2);
    color value(double u, double v, const vec3h& p) const override;

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class image_texture : public texture {
public:
    image_texture(const char* filename);
    color value(double u, double v, const vec3h& p) const override;

private:
    rtw_image image;
};

#endif // TEXTURE_H
