#ifndef MATH_H
#define MATH_H

double lerp(double start, double end, double t) {
    return start + t * (end - start);
}

#endif