#ifndef BVH_AGGREGATE_H
#define BVH_AGGREGATE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>  // For std::shared_ptr
#include "../utils.h"
#include "bvh_util.h"

class BVHAggregate {
private:
    int max_prims_in_node;
    std::unique_ptr<BVHTreeNode> head;
public:
    BVHAggregate(); // Default constructor declaration
    BVHAggregate(std::vector<std::shared_ptr<hittable>> objs, int max_prims) 
    : max_prims_in_node(max_prims) {
        //  collect prims and add them to list of BVH Primitives

        std::vector<BVHPrimitive> bvhPrimitives(objs.size());
        for (size_t i = 0; i < objs.size(); ++i) {
            bvhPrimitives[i] = BVHPrimitive(i, (*objs[i]).bounds(), objs[i]); // pointer !!!!!!!!!
        }

        // Move ownership of the result from sah_recursive to 'head'
        if (objs.size() != 0) {
            head = sah_recursive(bvhPrimitives);
        }

    }

    BVHTreeNode* get_head() const {
        return head.get();
    }

    std::unique_ptr<BVHTreeNode> sah_recursive(std::vector<BVHPrimitive>& bvhPrimitives) {
        std::unique_ptr<BVHTreeNode> root = std::make_unique<BVHTreeNode>();

        // Compute root bounding box (bounding box containing all primitives)
        Bounds3f rootBoundingBox;
        int i = 0;
        for (const BVHPrimitive& prim : bvhPrimitives) {
            rootBoundingBox = Union(rootBoundingBox, prim.bounds);
        }
        //std::cout << "bvh prims size: " << bvhPrimitives.size() <<  std::endl;

        //std::cout << "bounding box info: " <<  rootBoundingBox.surface_area() << " " <<rootBoundingBox.axis_length(0) <<" " <<rootBoundingBox.axis_length(1) << " " <<rootBoundingBox.axis_length(2) << std::endl    ;
        //print(rootBoundingBox.pmin);
        //print(rootBoundingBox.pmax);
        root->bounds = rootBoundingBox;
        // Find largest axis to monitor
        int largest_axis = rootBoundingBox.max_dimen();
        //std::cout << "yaur" << std::endl;

        //std::sort(bvhPrimitives.begin(), bvhPrimitives.end(), [largest_axis](const BVHPrimitive& a, const BVHPrimitive& b) {
        //    return a.Centroid()[largest_axis] < b.Centroid()[largest_axis];
        //});
        // Generate buckets
        const int num_buckets = 12;
        double bucket_width = rootBoundingBox.axis_length(largest_axis) / num_buckets;
        std::vector<BVHBucket> buckets(num_buckets);

        // For each primitive, find which bucket it is in by it's centroid position
        for (const BVHPrimitive& bvh_prim : bvhPrimitives) {
            int bucket_number = static_cast<int>(std::floor((bvh_prim.Centroid()[largest_axis] - rootBoundingBox.pmin[largest_axis]) / bucket_width));
            //std::cout << bucket_number << std::endl;
            bucket_number = std::min(std::max(bucket_number, 0), num_buckets - 1);
            buckets[bucket_number].num_prims += 1;
            // Expand the bound's of the grouping in the bucket
            buckets[bucket_number].bounds = Union(buckets[bucket_number].bounds, bvh_prim.bounds);
            if (bucket_number == 5) {
                //std::cout << "bucket 1 has this many prims; " << buckets[bucket_number].num_prims << std::endl;
                //print(buckets[bucket_number].bounds.pmin);
                //print(buckets[bucket_number].bounds.pmax);
            }
        }
        std::vector <BVHBucket> clamped_buckets;
        for (int i = 0; i < num_buckets; i++) {
            if (buckets[i].num_prims > 0) {
                clamped_buckets.push_back(buckets[i]);
            }
        }


        double num_prims_left = 0;
        int num_splits = clamped_buckets.size() - 1;
        std::vector<double> costs(num_splits); // Initialize costs

        // Accumulate costs at each split from the left side
        Bounds3f boundBelow;
        for (int i = 0; i < num_splits; ++i) {
            //std::cout << "bucket" << i << clamped_buckets[i].bounds.surface_area() << std::endl;
            boundBelow = Union(boundBelow, clamped_buckets[i].bounds);
            num_prims_left += clamped_buckets[i].num_prims;
            costs[i] += num_prims_left * boundBelow.surface_area();
        }

        // Accumulate costs at each split from the right side
        int num_prims_right = 0;
        Bounds3f boundAbove;
        for (int i = num_splits; i >= 1; --i) {
            boundAbove = Union(boundAbove, clamped_buckets[i].bounds);
            num_prims_right += clamped_buckets[i].num_prims;
            costs[i - 1] += num_prims_right * boundAbove.surface_area();
        }

        // Find bucket with least cost
        int lowest_cost_split = -1;
        double lowest_cost = infinity;

        for (int i = 0; i < num_splits; ++i) {
            //std::cout << "cost " << i << " " << costs[i] << std::endl;
            if (costs[i] < lowest_cost) {
                lowest_cost = costs[i];
                lowest_cost_split = i;
            }
        }        

        // Compare to leaf costs (just the num of primitives)
        int leaf_cost = static_cast<int>(bvhPrimitives.size());
        //std::cout << "lowest cost split: " << lowest_cost_split <<  std::endl;

        lowest_cost = 1.f / 2.f + lowest_cost / rootBoundingBox.surface_area();
        //std::cout << "lowest cost: " << lowest_cost << " leaf cost: " << leaf_cost << std::endl;

        if (leaf_cost <= lowest_cost && leaf_cost <= max_prims_in_node) {
            // Turn into a leaf node
            root->prims = bvhPrimitives;
            return root;  // Return the unique_ptr directly
        } else {
            // Split primitives at the selected bucket
            double bucket_width = (rootBoundingBox.pmax[largest_axis] - rootBoundingBox.pmin[largest_axis]) / num_buckets;

            // Partition the primitives based on their centroid in the largest axis
            auto midIter = std::partition(
                bvhPrimitives.begin(), bvhPrimitives.end(),
                [=](const BVHPrimitive& prim) {
                    // Determine the bucket index based on the centroid's position
                    int bucket = static_cast<int>(std::floor((prim.Centroid()[largest_axis] - rootBoundingBox.pmin[largest_axis]) / bucket_width));
                    return bucket <= lowest_cost_split;  // Partition based on the split
                })
                ;
            
            std::vector<BVHPrimitive> leftPrims(bvhPrimitives.begin(), midIter);
            std::vector<BVHPrimitive> rightPrims(midIter, bvhPrimitives.end());

            if (lowest_cost_split == -1 || leftPrims.empty() || rightPrims.empty()) {
                auto mid = bvhPrimitives.begin() + bvhPrimitives.size() / 2;
                std::nth_element(bvhPrimitives.begin(), mid, bvhPrimitives.end(), 
                    [largest_axis](const BVHPrimitive& a, const BVHPrimitive& b) {
                        return a.Centroid()[largest_axis] < b.Centroid()[largest_axis];
                    });
                leftPrims.assign(bvhPrimitives.begin(), mid);
                rightPrims.assign(mid, bvhPrimitives.end());
            }
            
            // Allocate child nodes on right and left
            if (leftPrims.size() > 0) {
                root->left = sah_recursive(leftPrims);
            }
            if (rightPrims.size() > 0) {
                root->right = sah_recursive(rightPrims);
            }
        }

        return root;
    }
};

#endif
