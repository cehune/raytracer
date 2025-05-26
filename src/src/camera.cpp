#include "camera.h"
#include "color.h"
#include "utils.h"

camera::camera()
    : aspect_ratio(1.0), image_width(400), center(vec3h(0, 0, 0, 1)),
      lookat(vec3h(0, 0, -1, 0)), fov(90), viewport_height(2.0),
      aa_samples_per_px(10), ray_bounces(10), tilt_angle(0.0), focus_dist(10.0) {}

void camera::set_aspect_ratio(double ratio) {
    aspect_ratio = ratio;
}

void camera::set_img_width(int w) {
    image_width = w;
}

void camera::initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    pixel_samples_scale = 1.0 / aa_samples_per_px;
    tilt_angle = degrees_to_radians(tilt_angle);

    auto theta = degrees_to_radians(fov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h * focus_dist;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3h cam_direction = (center - lookat).normal_of();
    vec3h viewport_u = viewport_width * (cross_product(vec3h(0, 1, 0, 0), cam_direction).normal_of());
    vec3h viewport_v = -viewport_height * (cross_product(cam_direction, viewport_u).normal_of());

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Apply any tilt to the viewport
    apply_camera_rotation(cam_direction, pixel_delta_u, pixel_delta_v, tilt_angle);

    // Calculate the location of the top-left pixel
    auto viewport_upper_left = center - (focus_dist * cam_direction) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

void camera::apply_camera_rotation(vec3h& axis, vec3h& delta_u, vec3h& delta_v, double angle) {
    /*
        Util function to rotate the camera pixel_delta values by some axis about some angle.
        The angle should be the direction vector from the camera center to the lookat vector.
        Expects angle to be in radians.
    */
    transform rotation = rotateAxis(axis, angle);
    delta_u = apply_transform(rotation.m, delta_u);
    delta_v = apply_transform(rotation.m, delta_v);
}

color camera::ray_color(const ray& r, int depth, const hittable_list& world, BVHTreeNode* head) const {
    hit_record rec;

    if (depth <= 0) {
        return background;
    }

    // Set interval start at 0.001 to prevent a ray from bouncing with its own surface due to float roundoff
    if (world.intersect(head, r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        // Scatter rays based on material properties (e.g. diffuse, reflective)
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return hadamard_product(attenuation, ray_color(scattered, depth - 1, world, head)) + color_from_emission;
        }
        return color_from_emission;
    }

    // Background color (missed geometry)
    return background;
}

ray camera::generate_offset_ray(int i, int j, int sample_coord) {
    /*
        Generate a single sub-pixel ray offset using stratified sampling.
        sample_coord may be 0, 1, 2, 3 to generate a 2x2 grid of jittered samples.
    */
    sample_coord = sample_coord % 4;

    double i_offset = random_double(0, 0.5) * ((sample_coord == 0 || sample_coord == 2) ? 1 : -1);
    double j_offset = random_double(0, 0.5) * ((sample_coord == 0 || sample_coord == 3) ? 1 : -1);

    auto sample_center = pixel00_loc + ((i + i_offset) * pixel_delta_u) + ((j + j_offset) * pixel_delta_v);
    return ray(center, sample_center - center);
}

void camera::render(const hittable_list& world, BVHTreeNode* head) {
    initialize();
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // For all pixels, send out rays basically
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            vec3h pixel_color;
            for (int s = 0; s < aa_samples_per_px; s++) {
                ray offset_ray = generate_offset_ray(i, j, s);
                pixel_color += ray_color(offset_ray, ray_bounces, world, head);
            }
            write_color(std::cout, pixel_samples_scale * pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
