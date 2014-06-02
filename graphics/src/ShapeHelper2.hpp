/*
 * ShapeHelper2.hpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

#ifndef SHAPEHELPER2_HPP_
#define SHAPEHELPER2_HPP_


class ShapeHelper2
{
    enum ShapeType
    {
        PLANE,
        CUBE,
        CYLINDER,
        CONE,
        SPHERE,
        //TORUS
    };

    // BUILDER FUNCTIONS: they build the desired shape
    //-------------------------------------------------
    static void buildMesh(Mesh2& mesh, ShapeType shape_type, unsigned int num_slices = 10, unsigned int num_stacks = 10);

    static void buildPlane(std::string&            name,
                           Mesh2::IndexVector&     vIndices,
                           Mesh2::VertexVector&    vVertices);

    static void buildCube(std::string&             name,
                          Mesh2::IndexVector&      vIndices,
                          Mesh2::VertexVector&     vVertices);

    static void buildCylinder(std::string&         name,
                              Mesh2::IndexVector&  vIndices,
                              Mesh2::VertexVector& vVertices,
                              unsigned int         num_slices);

    static void buildCone(std::string&             name,
                          Mesh2::IndexVector&      vIndices,
                          Mesh2::VertexVector&     vVertices,
                          unsigned int             num_slices);

    static void buildSphere(std::string&           name,
                            Mesh2::IndexVector&    vIndices,
                            Mesh2::VertexVector&   vVertices,
                            unsigned int           num_slices,
                            unsigned int           num_stacks);
};


#endif /* SHAPEHELPER2_HPP_ */
