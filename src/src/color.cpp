#include "color.h"
#include "interval.h" 
#include <cmath>    

double gamma_correct(double linear_scale, double gamma_val) {
    return std::pow(linear_scale, 1.0 / gamma_val);
}

void write_color(std::ostream& out, const color& pixel_color) {
    /*
    Given an ofstream, writes the current pixel colour from 0-255
    for it's r g b values
    */
    auto r = gamma_correct(pixel_color.x);
    auto g = gamma_correct(pixel_color.y);
    auto b = gamma_correct(pixel_color.z);

    int rbyte = int(256 * clamp(r, 0.000, 0.999));
    int gbyte = int(256 * clamp(g, 0.000, 0.999));
    int bbyte = int(256 * clamp(b, 0.000, 0.999));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
