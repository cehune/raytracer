#ifndef CAMERA_H
#define CAMERA_H

#include "primitive_shapes/hittable.h"
#include "utils.h"
#include "geometry/matrix.h"
#include "geometry/transform.h"

class camera {
private:
    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale = 1.0 / aa_samples_per_px;
        tilt_angle = degrees_to_radians(tilt_angle);
        auto theta = degrees_to_radians(fov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;        // Determine viewport dimensions.
        auto viewport_width = viewport_height * (double(image_width)/image_height);
     
        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3h cam_direction = (center - lookat).normal_of();
        vec3h viewport_u = viewport_width * (cross_product(vec3h(0,1,0,0), cam_direction).normal_of());
        vec3h viewport_v = -viewport_height * (cross_product(cam_direction, viewport_u).normal_of());

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height; // no change

        apply_camera_rotation(cam_direction, pixel_delta_u, pixel_delta_v, tilt_angle);
        auto viewport_upper_left = center - (focus_dist * cam_direction) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    
        //auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle));
        //defocus_disk_u = defocus_radius* viewport_u.normal_of();
        //defocus_disk_v = defocus_radius* viewport_u.normal_of();
    }


    color ray_color(const ray& r, int depth, const hittable_list& world, BVHTreeNode* head) const {
        hit_record rec;
        if (depth <= 0) {
            return color(0,0,0,0);
        }

        // set interval start at 0.001 to prevent a ray from bouncing with it's start surface due to float roundoff
        if (world.intersect(head, r, interval(0.001, infinity), rec)) {
            //I think we need to put this reflective behaviour based on the material property of the element itself
            // This is semi lambertian, where we are basing new ray direction on the normal, but not true
            // Scatters rays towards the normals, but randomly
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return hadamard_product(attenuation, ray_color(scattered, depth-1, world, head));
            return color(0,0,0,0);
        }

        vec3h normal_dir = (r.direction().normal_of());
        auto a = 0.5*(normal_dir.y + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0, 0) + a*color(0.5, 0.7, 1.0, 0);
    }

public:
    double aspect_ratio;  // Ratio of image width over height
    double focal_length;
    double viewport_height;
    double pixel_samples_scale;
    double fov;
    int aa_samples_per_px;
    int image_width;  // Rendered image width in pixel count
    int image_height;   // Rendered image height
    int ray_bounces; // four by default
    double tilt_angle;
    //double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus
    vec3h center;         // Camera center, point
    vec3h lookat;         // Any point the camera is looking toward
    vec3h pixel00_loc;    // Location of pixel 0, 0, point
    vec3h   pixel_delta_u;  // Offset to pixel to the right, direction
    vec3h   pixel_delta_v;  // Offset to pixel below, direction
    //vec3h   defocus_disk_u;       // Defocus disk horizontal radius
    //vec3h   defocus_disk_v;       // Defocus disk vertical radius

    camera():
        aspect_ratio(1.0), image_width(400), center(vec3h(0,0,0,1)), lookat(vec3h(0,0,-1,0)), fov(90),
        viewport_height(2.0), aa_samples_per_px(10), ray_bounces(10), tilt_angle(0.0) {};

    void set_aspect_ratio(double ratio) {aspect_ratio = ratio;}
    void set_img_width(int w) {image_width = w;}
    

    void render(const hittable_list& world, BVHTreeNode* head) {
        initialize();
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                vec3h pixel_color = vec3h();
                for (int s = 0; s < aa_samples_per_px; s++) {
                    ray offset_ray = generate_offset_ray(i, j, s);
                    pixel_color += ray_color(offset_ray, ray_bounces, world, head);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n"; 
    }

    ray generate_offset_ray(int i, int j, int sample_coord) {
        /* Generate a single offset  
            sample_coord may be 0,1,2,3
        */
        sample_coord = sample_coord % 4;
        double i_offset = random_double(0, 0.5) * ((sample_coord == 0 || sample_coord == 2) ? 1 : -1);
        double j_offset = random_double(0, 0.5) * ((sample_coord == 0 || sample_coord == 3) ? 1 : -1);
        //auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto sample_center = pixel00_loc + ((i + i_offset) * pixel_delta_u) + ((j+j_offset) * pixel_delta_v);
        return ray(center, sample_center - center);
    }

    /*vec3h defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_unit_aperture_loc();
        return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    } */

    void apply_camera_rotation(vec3h& axis, vec3h &delta_u, vec3h &delta_v, double angle) {
        /*
        Util function to rotate the camera pixel_delta values by some axis about some angle
        The angle should be the direction vector from the camera center to the lookat vector.
        Expects angle to be in radians
        */
        transform rotation = rotateAxis(axis, angle);
        //std::cout << "war" << rotation.m[0][0] << std::endl;
        delta_u = apply_transform(rotation.m, delta_u);
        delta_v = apply_transform(rotation.m, delta_v);
    }
};

#endif