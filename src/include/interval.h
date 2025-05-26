#ifndef INTERVAL_H
#define INTERVAL_H

#include "utils.h"

class interval {
public:
    double min, max;

    interval(); 
    interval(double min, double max);

    double size() const;
    bool contains(double x) const;
    bool surrounds(double x) const;

    static const interval empty;
    static const interval universe;
};

#endif // INTERVAL_H
