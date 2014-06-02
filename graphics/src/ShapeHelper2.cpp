/*
 * ShapeHelper2.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "ShapeHelper2.hpp"

// Global includes
#include <unordered_map>    // std::unordered_map
#include <functional>       // std::function

// Custom hash function
size_t vertexHash(const Mesh2::Vertex& vertex)
{
    return (  std::hash<float>()(vertex.position[0])
            ^ std::hash<float>()(vertex.position[1])
            ^ std::hash<float>()(vertex.position[2])
            ^ std::hash<float>()(vertex.normal[0])
            ^ std::hash<float>()(vertex.normal[1])
            ^ std::hash<float>()(vertex.normal[2])
            ^ std::hash<float>()(vertex.tex_coords[0])
            ^ std::hash<float>()(vertex.tex_coords[1])
            ^ std::hash<float>()(vertex.tex_coords[2]));
}



/**
* @brief Builder function
*
* It generates the Mesh for the given shape
*
* @oaram mesh       The Mesh object
* @oaram shape_type Shape to build (cube, sphere...)
* @param num_slices To be used only with cylinders, cones and spheres.
* @param num_stacs  To be used with spheres only.
*
* @return The Mesh
*/
void ShapeHelper2::buildMesh(Mesh2& mesh, ShapeType shape_type, unsigned int num_slices = 10, unsigned int num_stacks = 10)
{
    std::string         shape_name;
    Mesh2::IndexVector  vIndices;
    Mesh2::VertexVector vVertices;

    switch(shape_type)
    {
        case PLANE:
            ShapeHelper2::buildPlane(shape_name, vIndices, vVertices);
            break;

        case CUBE:
            ShapeHelper2::buildCube(shape_name, vIndices, vVertices);
            break;

        case CYLINDER:
            ShapeHelper2::buildCylinder(shape_name, vIndices, vVertices, num_slices);
            break;

        case CONE:
            ShapeHelper2::buildCone(shape_name, vIndices, vVertices, num_slices);
            break;

        case SPHERE:
            ShapeHelper2::buildSphere(shape_name, vIndices, vVertices, num_slices, num_stacks);
            break;

        default:
            std::cout << "Error: Shape type " << shape_type << " not handled by 'buildMesh'" << std::endl;
            break;
    }
}

void ShapeHelper2::buildPlane(std::string&         name,
                              Mesh2::IndexVector&  vIndices,
                              Mesh2::VertexVector& vVertices)
{
    name = std::string("Plane");
}



void ShapeHelper2::buildCube(std::string&         name,
                             Mesh2::IndexVector&  vIndices,
                             Mesh2::VertexVector& vVertices)
{

}



void ShapeHelper2::buildCylinder(std::string&         name,
                                 Mesh2::IndexVector&  vIndices,
                                 Mesh2::VertexVector& vVertices,
                                 unsigned int         num_slices)
{

}



void ShapeHelper2::buildCone(std::string&         name,
                             Mesh2::IndexVector&  vIndices,
                             Mesh2::VertexVector& vVertices,
                             unsigned int         num_slices)
{

}



void ShapeHelper2::buildSphere(std::string&         name,
                               Mesh2::IndexVector&  vIndices,
                               Mesh2::VertexVector& vVertices,
                               unsigned int         num_slices,
                               unsigned int         num_stacks)
{

}

