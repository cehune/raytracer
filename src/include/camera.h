#ifndef CAMERA_H
#define CAMERA_H

#include "primitive_shapes/hittable.h"
#include "primitive_shapes/hittable_list.h"
#include "acceleration/bvh_aggregate.h"
#include "geometry/vec3.h"
#include "geometry/transform.h"
#include "utils.h"
#include "interval.h"
#include <iostream>

class camera {
private:
    void initialize();
    color ray_color(const ray& r, int depth, const hittable_list& world, BVHTreeNode* head) const;
    void apply_camera_rotation(vec3h& axis, vec3h& delta_u, vec3h& delta_v, double angle);

public:
    double aspect_ratio;        // Ratio of image width over height
    double focal_length;
    double viewport_height;
    double pixel_samples_scale;
    double fov;
    int aa_samples_per_px;      // Anti-aliasing: samples per pixel
    int image_width;            // Obviously the image width
    int image_height;           // Same thing for height
    int ray_bounces;            // Max recursion depth for ray bounces
    double tilt_angle;          // Camera tilt in radians
    double focus_dist = 10;     // Distance from camera lookfrom point to plane of perfect focus

    vec3h center;               // Camera center (eye point)
    vec3h lookat;               // Point camera is looking at
    vec3h pixel00_loc;          // Location of pixel 0,0 in world space
    vec3h pixel_delta_u;        // Horizontal pixel spacing in world space
    vec3h pixel_delta_v;        // Vertical pixel spacing in world space
    color background;           // Background color if no hit

    camera();

    void set_aspect_ratio(double ratio);
    void set_img_width(int w);
    void render(const hittable_list& world, BVHTreeNode* head);
    ray generate_offset_ray(int i, int j, int sample_coord);
};

#endif // CAMERA_H
