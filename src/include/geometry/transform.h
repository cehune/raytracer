/*
General transformation utils for generating a transformation matrix
*/ 

#include "matrix.h"
#include "vec3.h"

class transform {
public:
    transform();
    transform(const squareMatrix<4>& m) : m(m) {}
    squareMatrix<4> m;

    transform translation(vec3h delta);
    transform scale(double x, double y, double z);
    transform rotateX(double theta);
    transform rotateY(double theta);
    transform rotateZ(double theta); // :P from khush
    transform lookAt(vec3h pos, vec3h look, vec3h up);
    transform orthographic(double znear, double zfar);
    transform perspective(double fov, double znear, double zfar);
};

vec3h apply_transform(squareMatrix<4> &transform, vec3h u) {
    return u*transform;
}

transform translate(vec3h delta) {
    /*
    Generates a matrix transform that will translate a point vector by the delta vector.
    */
    const double m[4][4] = {
        {1, 0, 0, delta.x},
        {0, 1, 0, delta.y},
        {0, 0, 1, delta.z},
        {0, 0, 0, 1      }
    };
    return transform(squareMatrix<4>(m));
}

transform scale(double x, double y, double z) {
    /*
    Generates a matrix transform that applies a scale to the three axis' of a vector. 
    */
    const double m[4][4] = {
        {x, 0, 0, 0},
        {0, y, 0, 0},
        {0, 0, z, 0},
        {0, 0, 0, 1}
    };
    return transform(squareMatrix<4>(m));
}

transform rotateX(double theta) {
    /*
    Generates a matrix transform that applies a rotation about the 'x' axis. 
    */
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    const double m[4][4] = {
        {1, 0,         0,          0},
        {0, cos_theta, -sin_theta, 0},
        {0, sin_theta, cos_theta,  0},
        {0, 0,         0,          1}
    };
    return transform(squareMatrix<4>(m));
}

transform rotateY(double theta) {
    /*
    Generates a matrix transform that applies a rotation about the 'y' axis. 
    */
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    const double m[4][4] = {
        {cos_theta,  0, sin_theta, 0},
        {0,          1, 0,         0},
        {-sin_theta, 0, cos_theta, 0},
        {0,          0, 0,         1}
    };
    return transform(squareMatrix<4>(m));
}

transform rotateZ(double theta) {
    /*
    Generates a matrix transform that applies a rotation about the 'z' axis. 
    */
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    const double m[4][4] = {
        {cos_theta, -sin_theta, 0, 0},
        {sin_theta, cos_theta,  0, 0},
        {0,         0,          1, 0},
        {0,         0,          0, 1}
    };
    return transform(squareMatrix<4>(m));
}

transform rotateAxis(vec3h axis, double theta) {
    /*
    Generates a matrix transform that applies a rotation about the 'axis' axis. 
     */
    axis = axis.normal_of(); // Ensure the axis is a unit vector
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);
    double one_minus_cos = 1.0 - cos_theta;

    const double x = axis.x;
    const double y = axis.y;
    const double z = axis.z;

    const double m[4][4] = {
        {cos_theta + x * x * one_minus_cos,      x * y * one_minus_cos - z * sin_theta, x * z * one_minus_cos + y * sin_theta, 0},
        {y * x * one_minus_cos + z * sin_theta, cos_theta + y * y * one_minus_cos,      y * z * one_minus_cos - x * sin_theta, 0},
        {z * x * one_minus_cos - y * sin_theta, z * y * one_minus_cos + x * sin_theta, cos_theta + z * z * one_minus_cos,      0},
        {0,                                     0,                                     0,                                     1}
    };

    return transform(squareMatrix<4>(m));
}

transform lookAt(vec3h pos, vec3h look, vec3h up) {
    vec3h forward = (look - pos).normal_of();
    vec3h right = (cross_product(up, forward)).normal_of();
    vec3h new_up = cross_product(forward, right);

    const double m[4][4] = {
        {right.x,   right.y,   right.z,   -dot(right, pos)},
        {new_up.x,  new_up.y,  new_up.z,  -dot(new_up, pos)},
        {forward.x, forward.y, forward.z, -dot(forward, pos)},
        {0,         0,         0,         1}
    };
    return transform(squareMatrix<4>(m));
}

transform orthographic(double znear, double zfar) {
    double depth = zfar - znear;
    const double m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -2 / depth, -(zfar + znear) / depth},
        {0, 0, 0, 1}
    };
    return transform(squareMatrix<4>(m));
}

transform perspective(double fov, double znear, double zfar) {
    double tan_half_fov = std::tan(fov / 2.0);
    double depth = zfar - znear;

    const double m[4][4] = {
        {1 / tan_half_fov, 0, 0, 0},
        {0, 1 / tan_half_fov, 0, 0},
        {0, 0, -(zfar + znear) / depth, -2 * zfar * znear / depth},
        {0, 0, -1, 0}
    };
    return transform(squareMatrix<4>(m));
}


