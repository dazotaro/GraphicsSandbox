/*
 * ShapeHelper2.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "ShapeHelper2.hpp"
#include "Mesh2.hpp"		// Mesh2

// Global includes
#include <unordered_map>    // std::unordered_map
#include <functional>       // std::function



// Custom hash function
size_t vertexHash(const ShapeHelper2::Vertex& vertex)
{
    return (  std::hash<float>()(vertex.position[0])
            ^ std::hash<float>()(vertex.position[1])
            ^ std::hash<float>()(vertex.position[2])
            ^ std::hash<float>()(vertex.normal[0])
            ^ std::hash<float>()(vertex.normal[1])
            ^ std::hash<float>()(vertex.normal[2])
            ^ std::hash<float>()(vertex.tex_coords[0])
            ^ std::hash<float>()(vertex.tex_coords[1]));
}

// TYPEDEFS
typedef std::unordered_map<ShapeHelper2::Vertex, int, std::function<decltype(vertexHash)> > VertexHashMap;
typedef VertexHashMap::const_iterator VertexHashMapConstIter;


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
void ShapeHelper2::buildMesh(Mesh2& mesh, ShapeType shape_type, unsigned int num_slices, unsigned int num_stacks)
{
    std::string  shape_name;
    IndexVector  vIndices;
    VertexVector vVertices;

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
            std::printf("Error: Shape type %i not handled by 'buildMesh'\n", shape_type);
            break;
    }

    Mesh2::PositionList vPositions;
    Mesh2::NormalList   vNormals;
    Mesh2::TexCoordList vTexCoords;
    Mesh2::ColorList    vColors;
    Mesh2::TangentList  vTangents;

    JU::uint32 vertex_count(vVertices.size());

    vPositions.reserve(vertex_count);
    vNormals.reserve(vertex_count);
    vTexCoords.reserve(vertex_count);
    vColors.reserve(vertex_count);
    vTangents.reserve(vertex_count);

    for (VertexVectorConstIter iter = vVertices.begin(); iter != vVertices.end(); ++iter)
    {
    	vPositions.push_back(glm::vec3(iter->position[0], iter->position[1], iter->position[2]));
    	vNormals.push_back  (glm::vec3(iter->normal[0], iter->normal[1], iter->normal[2]));
    	vTexCoords.push_back(glm::vec2(iter->tex_coords[0], iter->tex_coords[1]));
    	vColors.push_back   (glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    	vTangents.push_back (glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    Mesh2::FaceList vFaces;
    vFaces.reserve(vIndices.size());

    for (IndexVectorConstIter iter = vIndices.begin(); iter != vIndices.end(); iter += 3)
    {
		vFaces.push_back(Mesh2::TriangleFace(*iter, *(iter + 1), *(iter + 2),   // Positions
										   *iter, *(iter + 1), *(iter + 2),   // Normals
										   *iter, *(iter + 1), *(iter + 2))); // Texture Coordinates
    }

    mesh = Mesh2(shape_name, vPositions, vNormals, vTangents, vColors, vTexCoords, vFaces);
}

void ShapeHelper2::buildPlane(std::string&  name,
                              IndexVector&  vIndices,
                              VertexVector& vVertices)
{
    name = std::string("Plane");
    vIndices.clear();
    vVertices.clear();

    JU::uint32 vertex_count = 0;		// Vertex counter
    JU::uint32 vertex_index;			// Vertex index
    Vertex vertex;						// Vertex data
    VertexHashMap vtx_hashmap(8);		// Hash map to keep track of uniqueness of vertices and their indices
    VertexHashMapConstIter vtx_iter;

    // Triangle 0
    // ----------
    // V0
    vertex = (-0.5f, 0.5f, 0.0f, // position
    		   1.0f, 0.0f, 0.0f, // normal
    		   0.0f, 1.0f);		 // texture coordinates

    // Does this vertex already exist?
    vtx_iter = vtx_hashmap.find(vertex);
    if (vtx_iter == vtx_hashmap.end())
    {
    	vtx_hashmap[vertex] = vertex_index = vertex_count++;
    	vVertices.push_back(vertex);
    }
    else
    	vertex_index = vtx_iter->second;

	vIndices.push_back(vertex_index);

    // V1
    vertex = (-0.5f, -0.5f, 0.0f, // position
    		   1.0f,  0.0f, 0.0f, // normal
    		   0.0f,  0.0f);	  // texture coordinates

    // Does this vertex already exist?
    vtx_iter = vtx_hashmap.find(vertex);
    if (vtx_iter == vtx_hashmap.end())
    {
    	vtx_hashmap[vertex] = vertex_index = vertex_count++;
    	vVertices.push_back(vertex);
    }
    else
    	vertex_index = vtx_iter->second;

	vIndices.push_back(vertex_index);

    // V2
    vertex = ( 0.5f, -0.5f, 0.0f, // position
    		   1.0f,  0.0f, 0.0f, // normal
    		   1.0f,  0.0f);	  // texture coordinates

    // Does this vertex already exist?
    vtx_iter = vtx_hashmap.find(vertex);
    if (vtx_iter == vtx_hashmap.end())
    {
    	vtx_hashmap[vertex] = vertex_index = vertex_count++;
    	vVertices.push_back(vertex);
    }
    else
    	vertex_index = vtx_iter->second;

	vIndices.push_back(vertex_index);

    // Triangle 1
    // ----------
    // V0
    vertex = (-0.5f, 0.5f, 0.0f, // position
    		   1.0f, 0.0f, 0.0f, // normal
    		   0.0f, 1.0f);		 // texture coordinates

    // Does this vertex already exist?
    vtx_iter = vtx_hashmap.find(vertex);
    if (vtx_iter == vtx_hashmap.end())
    {
    	vtx_hashmap[vertex] = vertex_index = vertex_count++;
    	vVertices.push_back(vertex);
    }
    else
    	vertex_index = vtx_iter->second;

	vIndices.push_back(vertex_index);

    // V1
    vertex = ( 0.5f, -0.5f, 0.0f, // position
    		   1.0f,  0.0f, 0.0f, // normal
    		   1.0f,  0.0f);	  // texture coordinates

    // Does this vertex already exist?
    vtx_iter = vtx_hashmap.find(vertex);
    if (vtx_iter == vtx_hashmap.end())
    {
    	vtx_hashmap[vertex] = vertex_index = vertex_count++;
    	vVertices.push_back(vertex);
    }
    else
    	vertex_index = vtx_iter->second;

	vIndices.push_back(vertex_index);

    // V2
    vertex = ( 0.5f,  0.5f, 0.0f, // position
    		   1.0f,  0.0f, 0.0f, // normal
    		   1.0f,  1.0f);	  // texture coordinates

    // Does this vertex already exist?
    vtx_iter = vtx_hashmap.find(vertex);
    if (vtx_iter == vtx_hashmap.end())
    {
    	vtx_hashmap[vertex] = vertex_index = vertex_count++;
    	vVertices.push_back(vertex);
    }
    else
    	vertex_index = vtx_iter->second;

	vIndices.push_back(vertex_index);
}



void ShapeHelper2::buildCube(std::string&  name,
                             IndexVector&  vIndices,
                             VertexVector& vVertices)
{

}



void ShapeHelper2::buildCylinder(std::string&  name,
                                 IndexVector&  vIndices,
                                 VertexVector& vVertices,
                                 unsigned int  num_slices)
{

}



void ShapeHelper2::buildCone(std::string&  name,
                             IndexVector&  vIndices,
                             VertexVector& vVertices,
                             unsigned int  num_slices)
{

}



void ShapeHelper2::buildSphere(std::string&  name,
                               IndexVector&  vIndices,
                               VertexVector& vVertices,
                               unsigned int  num_slices,
                               unsigned int  num_stacks)
{

}

