#ifndef CAMERA_H
#define CAMERA_H

#include "primitive_shapes/hittable.h"
#include "utils.h"

class camera {
private:
    double aspect_ratio;  // Ratio of image width over height
    double focal_length;
    double viewport_height;
    double pixel_samples_scale;
    int aa_samples_per_px;
    int image_width;  // Rendered image width in pixel count
    int image_height;   // Rendered image height
    int ray_bounces; // four by default
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale = 1.0 / aa_samples_per_px;
        // Determine viewport dimensions.

        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }
    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        if (depth <= 0) {
            return color(0,0,0);
        }

        // set interval start at 0.001 to prevent a ray from bouncing with it's start surface due to float roundoff
        if (world.hit(r, interval(0.001, infinity), rec)) {
            //I think we need to put this reflective behaviour based on the material property of the element itself
            // This is semi lambertian, where we are basing new ray direction on the normal, but not true
            // Scatters rays towards the normals, but randomly
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 normal_dir = (r.direction().normal_of());
        auto a = 0.5*(normal_dir.y + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

public:
    camera(double aspect_rat, int img_width, vec3& c, double focal_len, double viewpt_height, int samples_per_px,
        int max_ray_bounces):
        aspect_ratio(aspect_rat), image_width(img_width), center(c), focal_length(focal_len),
        viewport_height(viewpt_height), aa_samples_per_px(samples_per_px), ray_bounces(max_ray_bounces) {};
    
    camera():
        aspect_ratio(1.0), image_width(400), center(vec3(0,0,0)), focal_length(1.0),
        viewport_height(2.0), aa_samples_per_px(10), ray_bounces(4) {};

    void set_aspect_ratio(double ratio) {aspect_ratio = ratio;}
    void set_img_width(int w) {image_width = w;}
    

    void render(const hittable& world) {
        initialize();
        int jitter_regions = aa_samples_per_px <= 4 ? 4 : 8; // for anti-aliasing jitter algo

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                vec3 pixel_color = vec3();

                for (int s = 0; s < aa_samples_per_px; s++) {
                    ray offset_ray = generate_offset_ray(i, j, s);
                    pixel_color += ray_color(offset_ray, ray_bounces, world);
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

        auto sample_center = pixel00_loc + ((i + i_offset) * pixel_delta_u) + ((j+j_offset) * pixel_delta_v);
        return ray(center, sample_center - center);
    }
};

#endif