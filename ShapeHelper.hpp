#ifndef __SHAPE_BUILDER__
#define __SHAPE_BUILDER__

#include <vector>       // stl vector and iterator
#include "Mesh.hpp"     // Mesh, Mesh::Face

namespace Graphics
{
    enum TexMapMode
    {
        PLANAR,
        CYLINDRICAL,
        SPHERICAL
    };
    
    enum ShapeType
    {
        PLANE,
        CUBE,
        CYLINDER,
        CONE,
        SPHERE,
        //TORUS
    };

    // Colors
    const glm::vec4 RED    (1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 GREEN  (0.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 BLUE   (0.0f, 0.0f, 1.0f, 1.0f);
    const glm::vec4 YELLOW (1.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 PURPLE (1.0f, 0.0f, 1.0f, 1.0f);
    const glm::vec4 GRAY   (0.4f, 0.4f, 0.4f, 1.0f);


    // BUILDER FUNCTIONS: they build the desired shape
    //-------------------------------------------------
    Mesh buildMesh(ShapeType shape_type, unsigned int num_slices = 10, unsigned int num_stacks = 10);

    void buildPlane(std::string        &name,
                    Mesh::PositionList &positions,
                    Mesh::ColorList    &colors,
                    Mesh::TexCoordList &tex_coords,
                    Mesh::FaceList     &faces);

    void buildCube(std::string        &name,
                   Mesh::PositionList &positions,
                   Mesh::ColorList    &colors,
                   Mesh::TexCoordList &tex_coords,
                   Mesh::FaceList     &faces);

    void buildCylinder(std::string        &name,
                       Mesh::PositionList &positions,
                       Mesh::ColorList    &colors,
                       Mesh::TexCoordList &tex_coords,
                       Mesh::FaceList     &faces,
                       unsigned int        num_slices);

    void buildCone(std::string        &name,
                   Mesh::PositionList &positions,
                   Mesh::ColorList    &colors,
                   Mesh::TexCoordList &tex_coords,
                   Mesh::FaceList     &faces,
                   unsigned int        num_slices);

    void buildSphere(std::string        &name,
                     Mesh::PositionList &positions,
                     Mesh::ColorList    &colors,
                     Mesh::TexCoordList &tex_coords,
                     Mesh::FaceList     &faces,
                     unsigned int        num_slices,
                     unsigned int        num_stacks);

    void computeNormals(const Mesh::PositionList &positions,
                        const Mesh::FaceList     &faces,
                        Mesh::NormalList         &normals);

    void computeTangents(const Mesh::PositionList &positions,
                         const Mesh::FaceList     &faces,
                         const Mesh::NormalList   &normals,
                         const Mesh::TexCoordList &tex_coords,
                         Mesh::TangentList        &tangents);

} // End Graphics namespace

#endif
