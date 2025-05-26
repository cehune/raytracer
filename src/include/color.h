#ifndef COLOR_H
#define COLOR_H

#include "geometry/vec3.h"
#include <iostream>

// Alias vec3h to color
using color = vec3h;

// Gamma correction function
double gamma_correct(double linear_scale, double gamma_val = 2.2);

// Write RGB color to output stream (in 0–255 range)
void write_color(std::ostream& out, const color& pixel_color);

#endif // COLOR_H
