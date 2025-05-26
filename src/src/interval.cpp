#include "interval.h"

// Default constructor, invalid interval if nothing given
interval::interval() : min(+infinity), max(-infinity) {}

interval::interval(double min_val, double max_val) : min(min_val), max(max_val) {}

double interval::size() const {
    // Length of the interval
    return max - min;
}

bool interval::contains(double x) const {
    // Check if x is inside of the interval (includes edge)
    return min <= x && x <= max;
}

bool interval::surrounds(double x) const {
    // Checks if x is strictly inside of the interval
    return min < x && x < max;
}

// Define static instances
const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
