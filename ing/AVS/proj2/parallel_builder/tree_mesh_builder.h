/**
 * @file    tree_mesh_builder.h
 *
 * @author  Ondřej Koumar <xkouma02@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    DATE
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include "../common/base_mesh_builder.h"

class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return this->triangles.data() ; }

    unsigned process_child_cube(const Vec3_t<float>& cube_offset, unsigned edge_size, const ParametricScalarField& field);

    std::vector<Triangle_t> triangles;
    const unsigned CUT_OFF_EDGE_LENGTH = 1;
};

#endif // TREE_MESH_BUILDER_H
