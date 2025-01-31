#ifndef TEST_BVH_H
#define TEST_BVH_H

#include <cassert>
#include <vector>
#include <iostream>
#include "../include/acceleration/bvh_aggregate.h" // Include your BVH header
#include "../include/primitive_shapes/hittable_list.h"
#include "../include/primitive_shapes/sphere.h"

void test_empty_primitives() {
    /* A BVH system with no primitives should have a null head pointer*/
    hittable_list world;
    BVHAggregate bvh(world.objects, 1);

    auto node = bvh.get_head();
    assert(node == nullptr);
    std::cout << "test_empty_primitives passed!\n";
}

void test_non_empty_primitives() {
    /* A BVH system with no primitives should have a null head pointer*/
    hittable_list world;
    world.add(make_shared<sphere>(vec3h(5.0, 0, 0.0, 1), 1.0));
    BVHAggregate bvh(world.objects, 1);

    auto node = bvh.get_head();
    assert(node != nullptr);
    std::cout << "test_non_empty_primitives passed!\n";
}

void test_leaf_node_creation() {
    /* Test using two prims and that it makes leaf nodes correctly*/
    hittable_list world;
    world.add(make_shared<sphere>(vec3h(5.0, 0, 0.0, 1), 1.0));
    world.add(make_shared<sphere>(vec3h(0.0, 0, 0.0, 1), 1.0));

    BVHAggregate bvh(world.objects, 1);
    auto node = bvh.get_head();

    // Check if the returned node is a leaf
    assert(node != nullptr && "Node should not be nullptr for valid primitives");
    assert(node->left.get()->isLeaf() && node->right.get()->isLeaf() && "Child nodes should be a leaf without children");
    assert(node->left.get()->prims.size() == 1  && "Children node should store one primitives");

    BVHAggregate bvh2(world.objects, 2);
    node = bvh2.get_head();

    // Check if the returned node is a leaf
    assert(node != nullptr && "Node should not be nullptr for valid primitives");
    assert(node->isLeaf() && "Node should be a leaf without children");

    std::cout << "test_leaf_node_creation passed!" << std::endl;
}


void test_multi_leaf_node_creation() {
    /* Test using three prims and that it makes leaf nodes correctly*/
    hittable_list world;
    world.add(make_shared<sphere>(vec3h(5.0, 0, 0.0, 1), 1.0));
    world.add(make_shared<sphere>(vec3h(0.0, 0, 0.0, 1), 0.4));
    world.add(make_shared<sphere>(vec3h(-1.0, 0, 0.0, 1), 0.4));

    BVHAggregate bvh(world.objects, 1);
    auto node = bvh.get_head();
    assert(!(node->left.get()->isLeaf()) && node->right.get()->isLeaf() && "Left node should be a split node");
    assert(node->left.get()->left != nullptr && "Left nodes left should be a non null node");

    // Same procedure, increase max prims per node
    BVHAggregate bvh2(world.objects, 2);
    node = bvh2.get_head();
    assert((node->left.get()->isLeaf()) && node->right.get()->isLeaf() && "Both child nodes should be leafs");
    assert(node->left.get()->prims.size() == 2 && "Children node should store one or more prims");

    std::cout << "test_multi_leaf_node_creation passed!" << std::endl;
}

int run_test_bvh() {
    std::cout << "\n Starting tests for /acceleration/bvh_aggregate\n\n";

    test_empty_primitives();
    test_non_empty_primitives();
    test_leaf_node_creation();
    test_multi_leaf_node_creation();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#endif