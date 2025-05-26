#ifndef TRANSFORM_H
#define TRANSFORM_H
/*
General transformation utils for generating a transformation matrix
*/ 

#include "matrix.h"
#include "vec3.h"

class transform {
public:
    transform();
    transform(const squareMatrix<4>& m);
    transform& operator=(const transform& t1);
    squareMatrix<4> m;
};

vec3h apply_transform(squareMatrix<4> &transform, vec3h u);

transform combine_transform(transform &t1, transform &t2);

transform combine_transform(transform &t1, transform &t2, transform &t3);

transform combine_transform(transform &t1, transform &t2, transform &t3, transform &t4);

transform translate(vec3h delta);

transform scale(double x, double y, double z);

transform rotateX(double theta);

transform rotateY(double theta);

transform rotateZ(double theta);

transform rotateAxis(vec3h axis, double theta);

transform lookAt(vec3h pos, vec3h look, vec3h up);

transform orthographic(double znear, double zfar);

transform perspective(double fov, double znear, double zfar);


#endif