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
size_t vec2Hash(const glm::vec2& vec)
{
    return (std::hash<float>()(vec[0]) ^ std::hash<float>()(vec[1]));
}

// Custom hash function
size_t vec3Hash(const glm::vec3& vec)
{
    return (std::hash<float>()(vec[0]) ^ std::hash<float>()(vec[1]) ^ std::hash<float>()(vec[2]));
}

// Custom hash function
size_t vertexIndicesHash(const Mesh2::VertexIndices& vertex)
{
    return (  std::hash<JU::uint32>()(vertex.position_)
    		^ std::hash<JU::uint32>()(vertex.normal_)
    		^ std::hash<JU::uint32>()(vertex.tex_));
}

bool vec3Equal(const glm::vec3& a, const glm::vec3& b)
{
	const JU::f32 EPSILON = 0.00001f;
	bool result = true;

	if (fabsf(a[0] - b[0]) > EPSILON || fabsf(a[1] - b[1]) > EPSILON || fabsf(a[2] - b[2]) > EPSILON)
		result = false;

	if (a[0] > 0 && b[0] > 0 && a[2] > 0 && b[2] > 0)
		std::printf("%f %f %f -- %f %f %f = %s\n", a[0], a[1], a[2], b[0], b[1], b[2], result ? "TRUE" : "FALSE");


	return true;
}

bool vec2Equal(const glm::vec2& a, const glm::vec2& b)
{
	const JU::f32 EPSILON = 0.00001f;

	if (fabsf(a[0] - b[0]) > EPSILON || fabsf(a[1] - b[1]) > EPSILON)
		return false;

	return true;
}

// TYPEDEFS
typedef std::unordered_map<glm::vec3, int, std::function<decltype(vec3Hash)>, std::function<decltype(vec3Equal)> > HashMapVec3;
typedef HashMapVec3::const_iterator HashMapVec3ConstIter;
typedef std::unordered_map<glm::vec2, int, std::function<decltype(vec2Hash)>, std::function<decltype(vec2Equal)> > HashMapVec2;
typedef HashMapVec2::const_iterator HashMapVec2ConstIter;
typedef std::unordered_map<Mesh2::VertexIndices, int, std::function<decltype(vertexIndicesHash)> > HashMapVertexIndices;
typedef HashMapVertexIndices::const_iterator HashMapVertexIndicesConstIter;



/**
* @brief Helper function
*
* Helper function to aid the buildShape functions in handling vertex duplication
*
* @oaram vertex             The new vertex data
* @oaram hpPositions    	A hash map with all the positions and their indices
* @param hpNormals          A hash map with all the normals and their indices
* @param hpTexCoords        A hash map with all the texture coordinates and their indices
* @param hpVertexIndices    A hash map with all the vertex indices (position, normal, tex) and their indices into the corresponding vector
* @param vPositions			Vector with all the vertex positions, indexed with the value stored in its corresponding hash map
* @param vNormals			Vector with all the vertex normals, indexed with the value stored in its corresponding hash map
* @param vTexCoords			Vector with all the vertex texture coordinates, indexed with the value stored in its corresponding hash map
* @param vVertexIndices		Vector with all the vertex indices(position, normal, tex), indexed with the value stored in its corresponding hash map
*
* @return The index to retrieve this vector from the vector of vertex indices (vVertexIndices)
*/
inline Mesh2::VertexIndex processVertex(const ShapeHelper2::Vertex& vertex,
						  	  	  	  	HashMapVec3& hpPositions,
						  	  	  	  	HashMapVec3& hpNormals,
						  	  	  	  	HashMapVec2& hpTexCoords,
						  	  	  	  	HashMapVertexIndices& hpVertexIndices,
						  	  	  	  	Mesh2::VectorPositions& vPositions,
						  	  	  	  	Mesh2::VectorNormals& vNormals,
						  	  	  	  	Mesh2::VectorTexCoords& vTexCoords,
						  	  	  	  	Mesh2::VectorVertexIndices& vVertexIndices)
{
    Mesh2::VertexIndex vertex_index;

    // POSITION
    //---------
    JU::uint32 pos_index;
    HashMapVec3ConstIter pos_iter = hpPositions.find(vertex.position_);
    if (pos_iter == hpPositions.end())
    {
    	hpPositions[vertex.position_] = pos_index = vPositions.size();
    	vPositions.push_back(vertex.position_);
    }
    else
    	pos_index = pos_iter->second;

    // NORMAL
    //---------
    JU::uint32 normal_index;
    HashMapVec3ConstIter normal_iter = hpNormals.find(vertex.normal_);
    if (normal_iter == hpNormals.end())
    {
    	hpNormals[vertex.normal_] = normal_index = vNormals.size();
    	vNormals.push_back(vertex.normal_);
    }
    else
    	normal_index = normal_iter->second;

    // TEXTURE COORDINATE
    //---------
    JU::uint32 tex_index;
    HashMapVec2ConstIter tex_iter = hpTexCoords.find(vertex.tex_coords_);
    if (tex_iter == hpTexCoords.end())
    {
    	hpTexCoords[vertex.tex_coords_] = tex_index = vTexCoords.size();
    	vTexCoords.push_back(vertex.tex_coords_);
    }
    else
    	tex_index = tex_iter->second;


    // VERTEX: Does this vertex already exist?
    Mesh2::VertexIndices vertex_indices (pos_index, normal_index, tex_index);
    HashMapVertexIndicesConstIter vtx_iter = hpVertexIndices.find(vertex_indices);
    if (vtx_iter == hpVertexIndices.end())
    {
        hpVertexIndices[vertex_indices] = vertex_index = vVertexIndices.size();
        vVertexIndices.push_back(vertex_indices);
    }
    else
        vertex_index = vtx_iter->second;

    return vertex_index;
}



/**
* @brief Helper function
*
* Helper function to aid the buildShape functions in inserting a triangle
*
* @oaram v0                 Vertex of quad
* @oaram v1                 Vertex of quad
* @oaram v2                 Vertex of quad
* @oaram v3                 Vertex of quad
* @oaram hpVertexIndices    A hash map with all the vertex indices per vertex-key
* @param vVertices          A vector with all the vertices
* @param vIndices           A vector with all the indices to the vertices
*
* @return The Mesh
*/
inline void addTriangle(const ShapeHelper2::Vertex& 	v0,
                        const ShapeHelper2::Vertex& 	v1,
                        const ShapeHelper2::Vertex& 	v2,
                        HashMapVec3& 					hpPositions,
						HashMapVec3& 					hpNormals,
						HashMapVec2& 					hpTexCoords,
						HashMapVertexIndices& 			hpVertexIndices,
						Mesh2::VectorPositions& 		vPositions,
						Mesh2::VectorNormals& 			vNormals,
						Mesh2::VectorTexCoords& 		vTexCoords,
						Mesh2::VectorVertexIndices& 	vVertexIndices,
						Mesh2::VectorTriangleIndices& 	vTriangleIndices)
{
	Mesh2::VertexIndex v0_index (processVertex(v0, hpPositions, hpNormals, hpTexCoords, hpVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices));
	Mesh2::VertexIndex v1_index (processVertex(v1, hpPositions, hpNormals, hpTexCoords, hpVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices));
	Mesh2::VertexIndex v2_index (processVertex(v2, hpPositions, hpNormals, hpTexCoords, hpVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices));

	vTriangleIndices.push_back(Mesh2::TriangleIndices(v0_index, v1_index, v2_index));
}



/**
* @brief Helper function
*
* Helper function to aid the buildShape functions in inserting a quad
*
* @oaram v0                 Vertex of quad
* @oaram v1                 Vertex of quad
* @oaram v2                 Vertex of quad
* @oaram v3                 Vertex of quad
* @oaram hpVertexIndices    A hash map with all the vertex indices per vertex-key
* @param vVertices          A vector with all the vertices
* @param vIndices           A vector with all the indices to the vertices
*
* @return The Mesh
*/
inline void addTriangulatedQuad(const ShapeHelper2::Vertex& v0,
                                const ShapeHelper2::Vertex& v1,
                                const ShapeHelper2::Vertex& v2,
                                const ShapeHelper2::Vertex& v3,
                                HashMapVec3& 				hpPositions,
                                HashMapVec3& 				hpNormals,
                                HashMapVec2& 				hpTexCoords,
                                HashMapVertexIndices& 		hpVertexIndices,
                                Mesh2::VectorPositions& 	vPositions,
                                Mesh2::VectorNormals& 		vNormals,
                                Mesh2::VectorTexCoords& 	vTexCoords,
                                Mesh2::VectorVertexIndices& vVertexIndices,
  							  Mesh2::VectorTriangleIndices& vTriangleIndices)
{
    addTriangle(v0, v1, v2, hpPositions, hpNormals, hpTexCoords, hpVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
    addTriangle(v0, v2, v3, hpPositions, hpNormals, hpTexCoords, hpVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
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
void ShapeHelper2::buildMesh(Mesh2& mesh, ShapeType shape_type, unsigned int num_slices, unsigned int num_stacks)
{
    std::string  				 shape_name;
	Mesh2::VectorPositions		 vPositions;    	//!< Vector of vertex coordinates
	Mesh2::VectorNormals      	 vNormals;      	//!< Vector of vertex normals
	Mesh2::VectorTexCoords    	 vTexCoords;    	//!< Vector of vertex texture coordinates
	Mesh2::VectorVertexIndices 	 vVertexIndices;  	//!< Vector of vertex indices (position, normal, texture coordinates)
	Mesh2::VectorTriangleIndices vTriangleIndices;	//!< Vector of triangle indices (v0, v1, v2)

    switch(shape_type)
    {
        case PLANE:
            ShapeHelper2::buildPlane(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
            break;

        case CUBE:
            ShapeHelper2::buildCube(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
            break;

        case CYLINDER:
            ShapeHelper2::buildCylinder(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices, num_slices);
            break;

        case CONE:
            ShapeHelper2::buildCone(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices, num_slices);
            break;

        case SPHERE:
            ShapeHelper2::buildSphere(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices, num_slices, num_stacks);
            break;

        default:
            std::printf("Error: Shape type %i not handled by 'buildMesh'\n", shape_type);
            break;
    }

    mesh = Mesh2(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
}

void ShapeHelper2::buildPlane(std::string&  				name,
		   	   	   	   	   	  Mesh2::VectorPositions&  	 	vPositions,
		   	   	   	   	   	  Mesh2::VectorNormals& 		vNormals,
		   	   	   	   	   	  Mesh2::VectorTexCoords&		vTexCoords,
		   	   	   	   	   	  Mesh2::VectorVertexIndices& 	vVertexIndices,
							  Mesh2::VectorTriangleIndices& vTriangleIndices)
{
    name = std::string("Plane");

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    HashMapVec3 hpPositions	(8, vec3Hash, vec3Equal);
    HashMapVec3 hpNormals  	(8, vec3Hash, vec3Equal);
    HashMapVec2 hpTexCoords (8, vec2Hash, vec2Equal);
    HashMapVertexIndices hpVertexIndices(8, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0(-0.5f,  0.5f, 0.0f, // position
               0.0f,  0.0f, 1.0f, // normal
               0.0f,  1.0f);      // texture coordinates
    Vertex v1(-0.5f, -0.5f, 0.0f, // position
               0.0f,  0.0f, 1.0f, // normal
               0.0f,  0.0f);      // texture coordinates
    Vertex v2( 0.5f, -0.5f, 0.0f, // position
               0.0f,  0.0f, 1.0f, // normal
               1.0f,  0.0f);      // texture coordinates
    Vertex v3( 0.5f,  0.5f, 0.0f, // position
               0.0f,  0.0f, 1.0f, // normal
               1.0f,  1.0f);      // texture coordinates

    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
}



void ShapeHelper2::buildCube(std::string&  					name,
		   	   	   	   	     Mesh2::VectorPositions&  		vPositions,
		   	   	   	   	     Mesh2::VectorNormals& 			vNormals,
		   	   	   	   	     Mesh2::VectorTexCoords&		vTexCoords,
		   	   	   	   	     Mesh2::VectorVertexIndices& 	vVertexIndices,
							  Mesh2::VectorTriangleIndices& vTriangleIndices)
{
    name = std::string("Cube");

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    HashMapVec3 hpPositions	(30, vec3Hash, vec3Equal);
    HashMapVec3 hpNormals  	(30, vec3Hash, vec3Equal);
    HashMapVec2 hpTexCoords (30, vec2Hash, vec2Equal);
    HashMapVertexIndices hpVertexIndices(30, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0, v1, v2, v3;

    // Face 0: normal (0, 0, 1)
    v0 = Vertex(-0.5f, 0.5f, 0.5f, // position
                 0.0f, 0.0f, 1.0f, // normal
                 0.0f, 1.0f);      // texture coordinates
    v1 = Vertex(-0.5f,-0.5f, 0.5f, // position
                 0.0f, 0.0f, 1.0f, // normal
                 0.0f, 0.0f);      // texture coordinates
    v2 = Vertex( 0.5f,-0.5f, 0.5f, // position
                 0.0f, 0.0f, 1.0f, // normal
                 1.0f, 0.0f);      // texture coordinates
    v3 = Vertex( 0.5f, 0.5f, 0.5f, // position
                 0.0f, 0.0f, 1.0f, // normal
                 1.0f, 1.0f);      // texture coordinates
    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

    // Face 1: normal (0, 0, -1)
    v0 = Vertex( 0.5f, 0.5f,-0.5f, // position
                 0.0f, 0.0f,-1.0f, // normal
                 0.0f, 1.0f);      // texture coordinates
    v1 = Vertex( 0.5f,-0.5f,-0.5f, // position
                 0.0f, 0.0f,-1.0f, // normal
                 0.0f, 0.0f);      // texture coordinates
    v2 = Vertex(-0.5f,-0.5f,-0.5f, // position
                 0.0f, 0.0f,-1.0f, // normal
                 1.0f, 0.0f);      // texture coordinates
    v3 = Vertex(-0.5f, 0.5f,-0.5f, // position
                 0.0f, 0.0f,-1.0f, // normal
                 1.0f, 1.0f);      // texture coordinates
    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

    // Face 2: normal (1, 0, 0)
    v0 = Vertex( 0.5f, 0.5f, 0.5f, // position
                 1.0f, 0.0f, 0.0f, // normal
                 0.0f, 1.0f);      // texture coordinates
    v1 = Vertex( 0.5f,-0.5f, 0.5f, // position
                 1.0f, 0.0f, 0.0f, // normal
                 0.0f, 0.0f);      // texture coordinates
    v2 = Vertex( 0.5f,-0.5f,-0.5f, // position
                 1.0f, 0.0f, 0.0f, // normal
                 1.0f, 0.0f);      // texture coordinates
    v3 = Vertex( 0.5f, 0.5f,-0.5f, // position
                 1.0f, 0.0f, 0.0f, // normal
                 1.0f, 1.0f);      // texture coordinates
    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

    // Face 3: normal (-1, 0, 0)
    v0 = Vertex(-0.5f, 0.5f,-0.5f, // position
                -1.0f, 0.0f, 0.0f, // normal
                 0.0f, 1.0f);      // texture coordinates
    v1 = Vertex(-0.5f,-0.5f,-0.5f, // position
                -1.0f, 0.0f, 0.0f, // normal
                 0.0f, 0.0f);      // texture coordinates
    v2 = Vertex(-0.5f,-0.5f, 0.5f, // position
                -1.0f, 0.0f, 0.0f, // normal
                 1.0f, 0.0f);      // texture coordinates
    v3 = Vertex(-0.5f, 0.5f, 0.5f, // position
                -1.0f, 0.0f, 0.0f, // normal
                 1.0f, 1.0f);      // texture coordinates
    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

    // Face 4: normal (0, 1, 0)
    v0 = Vertex( 0.5f, 0.5f,-0.5f, // position
                 0.0f, 1.0f, 0.0f, // normal
                 0.0f, 1.0f);      // texture coordinates
    v1 = Vertex(-0.5f, 0.5f,-0.5f, // position
                 0.0f, 1.0f, 0.0f, // normal
                 0.0f, 0.0f);      // texture coordinates
    v2 = Vertex(-0.5f, 0.5f, 0.5f, // position
                 0.0f, 1.0f, 0.0f, // normal
                 1.0f, 0.0f);      // texture coordinates
    v3 = Vertex( 0.5f, 0.5f, 0.5f, // position
                 0.0f, 1.0f, 0.0f, // normal
                 1.0f, 1.0f);      // texture coordinates
    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

    // Face 5: normal (0, -1, 0)
    v0 = Vertex( 0.5f,-0.5f, 0.5f, // position
                 0.0f,-1.0f, 0.0f, // normal
                 0.0f, 1.0f);      // texture coordinates
    v1 = Vertex(-0.5f,-0.5f, 0.5f, // position
                 0.0f,-1.0f, 0.0f, // normal
                 0.0f, 0.0f);      // texture coordinates
    v2 = Vertex(-0.5f,-0.5f,-0.5f, // position
                 0.0f,-1.0f, 0.0f, // normal
                 1.0f, 0.0f);      // texture coordinates
    v3 = Vertex( 0.5f,-0.5f,-0.5f, // position
                 0.0f,-1.0f, 0.0f, // normal
                 1.0f, 1.0f);      // texture coordinates
    addTriangulatedQuad(v0, v1, v2, v3,
    					hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
}



void ShapeHelper2::buildCylinder(std::string&  					name,
		   	   	   	   	   	   	 Mesh2::VectorPositions&  		vPositions,
		   	   	   	   	   	   	 Mesh2::VectorNormals& 			vNormals,
		   	   	   	   	   	   	 Mesh2::VectorTexCoords&		vTexCoords,
		   	   	   	   	   	   	 Mesh2::VectorVertexIndices& 	vVertexIndices,
								 Mesh2::VectorTriangleIndices& 	vTriangleIndices,
                                 unsigned int  					num_slices)
{
    // CONSTANTS
    const glm::vec3 ORIGIN (0.0f, 0.0f, 0.0f);          // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET (0.5f);                        // Distance from the ORIGIN to the top (or bottom)
    const float RADIUS   (0.5f);                        // RADIUS of the cylinder
    const float DELTA_THETA (2 * M_PI / num_slices);    // Increment of the angle from slice to slice
    const float DELTA_S(1.0F / num_slices);				// Increment of the s texture coordinate from slice to slice

    name = "Cylinder_" + num_slices;

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    HashMapVec3 hpPositions	(10 * num_slices, vec3Hash, vec3Equal);
    HashMapVec3 hpNormals  	(10 * num_slices, vec3Hash, vec3Equal);
    HashMapVec2 hpTexCoords (10 * num_slices, vec2Hash, vec2Equal);
    HashMapVertexIndices hpVertexIndices(10 * num_slices, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0, v1, v2, v3;

    // TOP DISK
    const glm::vec3 center_top (0.0f, 0.0f, Z_OFFSET);
    const glm::vec3 top_normal (0.0f, 0.0f, 1.0f);
    float theta = 0.0f;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta); 				JU::f32 y1 = RADIUS * sin(theta);
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, Z_OFFSET);
        glm::vec3 pos2 (x2, y2, Z_OFFSET);

        v0 = Vertex(center_top, // position
                    top_normal, // normal
                    glm::vec2(0.0f, 0.0f));      // texture coordinates
        v1 = Vertex(pos1, // position
                    top_normal, // normal
                    glm::vec2(x1, y1));      // texture coordinates
        v2 = Vertex(pos2, // position
                    top_normal, // normal
                    glm::vec2(x2, y2));      // texture coordinates
        addTriangle(v0, v1, v2,
        			hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // BOTTOM DISK
    const glm::vec3 center_bottom (0.0f, 0.0f, -Z_OFFSET);
    const glm::vec3 bottom_normal (0.0f, 0.0f, -1.0f);
    theta = 0.0f;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta); 				JU::f32 y1 = RADIUS * sin(theta);
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, -Z_OFFSET);
        glm::vec3 pos2 (x2, y2, -Z_OFFSET);

        v0 = Vertex(center_bottom, // position
                    bottom_normal, // normal
                    glm::vec2(0.0f, 0.0f));      // texture coordinates
        v1 = Vertex(pos2, // position
        			bottom_normal, // normal
                    glm::vec2(x2, y2));      // texture coordinates
        v2 = Vertex(pos1, // position
        			bottom_normal, // normal
                    glm::vec2(x1, y1));      // texture coordinates
        addTriangle(v0, v1, v2,
        			hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // SIDE OF THE CYLINDER
    theta = 0.0f;
    float tex_s = 0.0f;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta); 				JU::f32 y1 = RADIUS * sin(theta);
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, +Z_OFFSET);
        glm::vec3 pos2 (x2, y2, +Z_OFFSET);
        glm::vec3 norm1 (glm::normalize(pos1 - center_top));
        glm::vec3 norm2 (glm::normalize(pos2 - center_top));
        JU::f32 s1 = slice * DELTA_S;
        JU::f32 s2 = (slice + 1) * DELTA_S;

        v0 = Vertex(glm::vec3(x1, y1, +Z_OFFSET), // position
                    norm1, // normal
                    glm::vec2(s1, 1.0f));      // texture coordinates
        v1 = Vertex(glm::vec3(x1, y1, -Z_OFFSET), // position
        			norm1, // normal
                    glm::vec2(s1, 0.0f));      // texture coordinates
        v2 = Vertex(glm::vec3(x2, y2, -Z_OFFSET), // position
        			norm2, // normal
                    glm::vec2(s2, 0.0f));      // texture coordinates
        v3 = Vertex(glm::vec3(x2, y2, +Z_OFFSET), // position
        			norm2, // normal
                    glm::vec2(s2, 1.0f));      // texture coordinates
        addTriangulatedQuad(v0, v1, v2, v3,
        			hpPositions, hpNormals, hpTexCoords, hpVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
        tex_s += DELTA_S;
    }
}



void ShapeHelper2::buildCone(std::string&  					name,
	   	   	   	 	 	 	 Mesh2::VectorPositions&  		vPositions,
	   	   	   	 	 	 	 Mesh2::VectorNormals& 			vNormals,
	   	   	   	 	 	 	 Mesh2::VectorTexCoords&		vTexCoords,
	   	   	   	 	 	 	 Mesh2::VectorVertexIndices& 	vVertexIndices,
							 Mesh2::VectorTriangleIndices& 	vTriangleIndices,
	   	   	   	 	 	 	 unsigned int  					num_slices)
{

}



void ShapeHelper2::buildSphere(std::string&  				 name,
	   	   	   	 	 	 	   Mesh2::VectorPositions&  	 vPositions,
	   	   	   	 	 	 	   Mesh2::VectorNormals& 		 vNormals,
	   	   	   	 	 	 	   Mesh2::VectorTexCoords&		 vTexCoords,
	   	   	   	 	 	 	   Mesh2::VectorVertexIndices& 	 vVertexIndices,
	   	   	   	 	 	 	   Mesh2::VectorTriangleIndices& vTriangleIndices,
	   	   	   	 	 	 	   unsigned int  				 num_slices,
                               unsigned int  				 num_stacks)
{

}

