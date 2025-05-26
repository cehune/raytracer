#include "geometry/math.h"

double lerp(double start, double end, double t) {
    return start + t * (end - start);
}
