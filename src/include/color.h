#ifndef COLOR_H
#define COLOR_H

#include "geometry/vec3.h"
#include <iostream>
#include "interval.h"
using color = vec3h;
// TODO: confirm if alpha channel is required
inline double gamma_correct(double linear_scale, double gamma_val = 2.2) {
    // gamma val is 2.2 for typical displays
    return pow(linear_scale, 1.0/2.0);
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = gamma_correct(pixel_color.x);
    auto g = gamma_correct(pixel_color.y);
    auto b = gamma_correct(pixel_color.z);

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(256 * clamp(r, 0.000, 0.999));
    int gbyte = int(256 * clamp(g, 0.000, 0.999));
    int bbyte = int(256 * clamp(b, 0.000, 0.999));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif