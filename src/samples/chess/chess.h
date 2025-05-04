#include <iostream>
#include "../include/utils.h"
#include "../include/primitive_shapes/hittable.h"
#include "../include/primitive_shapes/hittable_list.h"
#include "../include/primitive_shapes/sphere.h"
#include "../include/camera.h"
#include "../include/acceleration/bvh_aggregate.h"
#include "../include/obj_loader.h"


void pin() {
    hittable_list world;
    auto tex = make_shared<checker_texture>(0.5, color(0.2, 0.2, 0.2), color(0.8, 0.8, 0.8));
    auto material_ground = make_shared<lambertian>(color(0.8, 0.3, 0.05, 0));
    auto difflight = make_shared<diffuse_light>(color(4,4,4));

    //ground
    world.add(make_shared<sphere>(vec3h(0,-1001,0, 1), 1000, make_shared<lambertian>(tex)));
    quadrilateral light(vec3h(-8,10,8,1), vec3h(20,0,0), vec3h(0,0,-20), difflight);

    world.add(&light);
    
    obj_loader loader;

    triangleMesh knight(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/knight.obj", knight);
    transform rot_x = rotateX(-pi / 2);
    transform rot_y = rotateY(-pi / 2.5);
    transform knight_rot_z = rotateZ(pi / 6);
    transform shift = translate(vec3h(3, -1, -5, 0));
    transform knight_scale = scale(0.6, 0.6, 0.6);
    knight.apply_total_transform(knight_scale);
    knight.apply_total_transform(rot_x);
    knight.apply_total_transform(rot_y);
    knight.apply_total_transform(knight_rot_z);
    knight.apply_total_transform(shift);
    world.add(&knight);

    triangleMesh pawn1(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/pawn.obj", pawn1);
    transform rot_z = rotateZ(pi / 6);
    rot_x = rotateX(pi / 2);
    rot_y = rotateY(pi / 16);
    shift = translate(vec3h(5.5, 3, -15, 0));
    transform scale_a = scale(0.35, 0.35, 0.35);
    pawn1.apply_total_transform(scale_a);
    pawn1.apply_total_transform(rot_x);
    pawn1.apply_total_transform(rot_z);
    pawn1.apply_total_transform(shift);
    world.add(&pawn1);

    triangleMesh pawn2(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/pawn.obj", pawn2);
    rot_z = rotateZ(-pi / 6);
    rot_x = rotateX(-pi / 2);
    rot_y = rotateY(-pi / 8);
    shift = translate(vec3h(0.5, -1.5, -14, 0));
    scale_a = scale(0.35, 0.35, 0.35);
    pawn2.apply_total_transform(scale_a);
    pawn2.apply_total_transform(rot_x);
    pawn2.apply_total_transform(rot_z);
    pawn2.apply_total_transform(shift);
    world.add(&pawn2);

    triangleMesh pawn3(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/pawn.obj", pawn3);
    rot_z = rotateZ(pi / 4);
    rot_x = rotateX(-pi / 2);
    rot_y = rotateY(-pi / 6);
    shift = translate(vec3h(-5, 3, -17, 0));
    scale_a = scale(0.35, 0.35, 0.35);
    pawn3.apply_total_transform(scale_a);
    pawn3.apply_total_transform(rot_x);
    pawn3.apply_total_transform(rot_z);
    pawn3.apply_total_transform(shift);
    world.add(&pawn3);

    triangleMesh rook(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/rook.obj", rook);
    rot_z = rotateZ(pi / 4);
    rot_x = rotateX(-pi / 2.3);
    rot_y = rotateY(-pi / 6);
    shift = translate(vec3h(-2, 1.5, -9, 0));
    scale_a = scale(0.35, 0.35, 0.35);
    rook.apply_total_transform(scale_a);
    rook.apply_total_transform(rot_x);
    rook.apply_total_transform(rot_z);
    rook.apply_total_transform(shift);
    world.add(&rook);

    triangleMesh king(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/king.obj", king);
    rot_z = rotateZ(-pi / 5);
    rot_x = rotateX(-pi / 2);
    rot_y = rotateY(-pi / 8);
    shift = translate(vec3h(-1.5, 0, -1, 0));
    scale_a = scale(0.35, 0.35, 0.35);
    king.apply_total_transform(scale_a);
    king.apply_total_transform(rot_x);
    king.apply_total_transform(rot_z);
    king.apply_total_transform(rot_y);
    king.apply_total_transform(shift);
    world.add(&king);

    triangleMesh queen(material_ground);
    loader.load_into_triangleMesh("src/resources/chess/queen.obj", queen);
    rot_z = rotateZ(-pi / 16);
    rot_x = rotateX(-pi / 1.75);
    rot_y = rotateY(pi / 5);
    shift = translate(vec3h(-0.2, 0, -6, 0));
    scale_a = scale(0.3, 0.3, 0.3);
    queen.apply_total_transform(scale_a);
    queen.apply_total_transform(rot_x);
    queen.apply_total_transform(rot_z);
    queen.apply_total_transform(rot_y);
    queen.apply_total_transform(shift);
    world.add(&queen);


    Bounds3f meshbounds = knight.bounds();
    BVHAggregate bvh(world.objects, 4);
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.aa_samples_per_px = 100;
    cam.ray_bounces         = 300;
    cam.fov     = 20;
    cam.center = vec3h(0, 0, 10);//vec3h(19,0,13);
    cam.lookat   = vec3h(0,0,0);
    cam.tilt_angle = 13.0;
    cam.focus_dist    = 10.0;
    cam.render(world, bvh.get_head());
}