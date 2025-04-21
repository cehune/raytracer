



// cl_renderer.cl
__kernel void render_kernel(
    __global float3* output, 
    __global hittable_list* world, 
    __global BVHTreeNode* head, 
    int image_width, 
    int image_height, 
    int aa_samples_per_px, 
    int ray_bounces,
    float pixel_samples_scale) 
{
    int i = get_global_id(0); // X (width)
    int j = get_global_id(1); // Y (height)
    
    if (i >= image_width || j >= image_height) return;

    vec3h pixel_color = {0.0f, 0.0f, 0.0f, 0.0f}; 
    
    for (int s = 0; s < aa_samples_per_px; s++) {
        ray offset_ray = generate_offset_ray(i, j, s);
        pixel_color += ray_color(offset_ray, ray_bounces, *world, head);
    }
    pixel_color = color(0.5f, 0.5f, 0.5f, 0);

    output[j * image_width + i] = (vec3h){ 0.5f, 0.5f, 0.5f };;
}
