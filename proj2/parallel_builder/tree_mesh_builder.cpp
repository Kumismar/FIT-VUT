/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Ondřej Koumar <xkouma02@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    DATE
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
    Vec3_t<float> start_offset(0.0f, 0.0f, 0.0f);
    unsigned total_triangles = 0;

    #pragma omp parallel default(none) shared(total_triangles, field, start_offset)
    {
        #pragma omp single nowait
        {
            total_triangles = this->process_child_cube(start_offset, this->mGridSize, field);
        }
    }

    return total_triangles;
}

unsigned TreeMeshBuilder::process_child_cube(const Vec3_t<float>& current_cube_offset, const unsigned edge_size, const ParametricScalarField& field)
{
    const static float half_sqrt_three = std::sqrt(3.0) / 2.0;
    const float max_distance_from_midpoint = half_sqrt_three * edge_size * this->mGridResolution + this->mIsoLevel; 
    const float half_edge_size_f = static_cast<float>(edge_size) / 2.0;

    Vec3_t<float> current_cube_mid_point = {
        (current_cube_offset.x + half_edge_size_f) * this->mGridResolution,
        (current_cube_offset.y + half_edge_size_f) * this->mGridResolution,
        (current_cube_offset.z + half_edge_size_f) * this->mGridResolution,
    };

    if (this->evaluateFieldAt(current_cube_mid_point, field) > max_distance_from_midpoint) {
        return 0;
    } 

    if (edge_size <= CUT_OFF_EDGE_LENGTH) {
        return this->buildCube(current_cube_offset, field);
    }

    unsigned total_triangles = 0;
    const unsigned half_edge_size = static_cast<unsigned>(half_edge_size_f);

    for (int i = 0; i < 8; i++) {
        #pragma omp task default(none) shared(total_triangles, field, current_cube_offset, half_edge_size) firstprivate(i) 
        {
            Vec3_t<float> child_cube_offset = {
                current_cube_offset.x + this->sc_vertexNormPos[i].x * half_edge_size,
                current_cube_offset.y + this->sc_vertexNormPos[i].y * half_edge_size,
                current_cube_offset.z + this->sc_vertexNormPos[i].z * half_edge_size
            };

            unsigned child_triangles = this->process_child_cube(child_cube_offset, half_edge_size, field);

            #pragma omp atomic
            total_triangles += child_triangles;
        }
    }
    #pragma omp taskwait

    return total_triangles;
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
     // NOTE: This method is called from "buildCube(...)"!

    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    for(unsigned i = 0; i < count; ++i) 
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    #pragma omp critical
    {
        this->triangles.push_back(triangle);
    }
}


