/*
 * ShapeHelper2.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "ShapeHelper2.hpp"
#include "Mesh2.hpp"		// Mesh2
#include "DebugGlm.hpp"

// Global includes
#include <unordered_map>    // std::unordered_map
#include <map>              // std::map
#include <functional>       // std::function


bool vec3Compare(const glm::vec3& a, const glm::vec3& b)
{
	const JU::f32 EPSILON = 0.00001f;

	if (fabsf(a[0] - b[0]) > EPSILON)
		return a[0] < b[0];

    if (fabsf(a[1] - b[1]) > EPSILON)
        return a[1] < b[1];

    if (fabsf(a[2] - b[2]) > EPSILON)
        return a[2] < b[2];

	return false;
}

bool vec2Compare(const glm::vec2& a, const glm::vec2& b)
{
    const JU::f32 EPSILON = 0.00001f;

    if (fabsf(a[0] - b[0]) > EPSILON)
        return a[0] < b[0];

    if (fabsf(a[1] - b[1]) > EPSILON)
        return a[1] < b[1];

    return false;
}

// Custom hash function
size_t vertexIndicesHash(const Mesh2::VertexIndices& vertex)
{
    return (  std::hash<JU::uint32>()(vertex.position_)
            ^ std::hash<JU::uint32>()(vertex.normal_)
            ^ std::hash<JU::uint32>()(vertex.tex_));
}
// TYPEDEFS
typedef std::map<glm::vec3, int, std::function<decltype(vec3Compare)> > MapVec3;
typedef MapVec3::const_iterator MapVec3ConstIter;
typedef std::map<glm::vec2, int, std::function<decltype(vec2Compare)> > MapVec2;
typedef MapVec2::const_iterator MapVec2ConstIter;
typedef std::unordered_map<Mesh2::VertexIndices, int, std::function<decltype(vertexIndicesHash)> > HashMapVertexIndices;
typedef HashMapVertexIndices::const_iterator HashMapVertexIndicesConstIter;



/**
* @brief Helper function
*
* Helper function to aid the buildShape functions in handling vertex duplication
*
* @oaram vertex             The new vertex data
* @oaram mPositions    	A hash map with all the positions and their indices
* @param mNormals          A hash map with all the normals and their indices
* @param mTexCoords        A hash map with all the texture coordinates and their indices
* @param hmVertexIndices    A hash map with all the vertex indices (position, normal, tex) and their indices into the corresponding vector
* @param vPositions			Vector with all the vertex positions, indexed with the value stored in its corresponding hash map
* @param vNormals			Vector with all the vertex normals, indexed with the value stored in its corresponding hash map
* @param vTexCoords			Vector with all the vertex texture coordinates, indexed with the value stored in its corresponding hash map
* @param vVertexIndices		Vector with all the vertex indices(position, normal, tex), indexed with the value stored in its corresponding hash map
*
* @return The index to retrieve this vector from the vector of vertex indices (vVertexIndices)
*/
inline Mesh2::VertexIndex processVertex(const ShapeHelper2::Vertex& vertex,
						  	  	  	  	MapVec3& mPositions,
						  	  	  	  	MapVec3& mNormals,
						  	  	  	  	MapVec2& mTexCoords,
						  	  	  	  	HashMapVertexIndices& hmVertexIndices,
						  	  	  	  	Mesh2::VectorPositions& vPositions,
						  	  	  	  	Mesh2::VectorNormals& vNormals,
						  	  	  	  	Mesh2::VectorTexCoords& vTexCoords,
						  	  	  	  	Mesh2::VectorVertexIndices& vVertexIndices)
{
    Mesh2::VertexIndex vertex_index;

    // POSITION
    //---------
    JU::uint32 pos_index;
    MapVec3ConstIter pos_iter = mPositions.find(vertex.position_);
    if (pos_iter == mPositions.end())
    {
    	mPositions[vertex.position_] = pos_index = vPositions.size();
    	vPositions.push_back(vertex.position_);
    }
    else
    	pos_index = pos_iter->second;

    // NORMAL
    //---------
    JU::uint32 normal_index;
    MapVec3ConstIter normal_iter = mNormals.find(vertex.normal_);
    if (normal_iter == mNormals.end())
    {
    	mNormals[vertex.normal_] = normal_index = vNormals.size();
    	vNormals.push_back(vertex.normal_);
    }
    else
    	normal_index = normal_iter->second;

    // TEXTURE COORDINATE
    //---------
    JU::uint32 tex_index;
    MapVec2ConstIter tex_iter = mTexCoords.find(vertex.tex_coords_);
    if (tex_iter == mTexCoords.end())
    {
    	mTexCoords[vertex.tex_coords_] = tex_index = vTexCoords.size();
    	vTexCoords.push_back(vertex.tex_coords_);
    }
    else
    	tex_index = tex_iter->second;


    // VERTEX: Does this vertex already exist?
    Mesh2::VertexIndices vertex_indices (pos_index, normal_index, tex_index);
    HashMapVertexIndicesConstIter vtx_iter = hmVertexIndices.find(vertex_indices);
    if (vtx_iter == hmVertexIndices.end())
    {
        hmVertexIndices[vertex_indices] = vertex_index = vVertexIndices.size();
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
* @oaram hmVertexIndices    A hash map with all the vertex indices per vertex-key
* @param vVertices          A vector with all the vertices
* @param vIndices           A vector with all the indices to the vertices
*
* @return The Mesh
*/
inline void addTriangle(const ShapeHelper2::Vertex& 	v0,
                        const ShapeHelper2::Vertex& 	v1,
                        const ShapeHelper2::Vertex& 	v2,
                        MapVec3& 					    mPositions,
						MapVec3& 					    mNormals,
						MapVec2& 					    mTexCoords,
						HashMapVertexIndices& 			hmVertexIndices,
						Mesh2::VectorPositions& 		vPositions,
						Mesh2::VectorNormals& 			vNormals,
						Mesh2::VectorTexCoords& 		vTexCoords,
						Mesh2::VectorVertexIndices& 	vVertexIndices,
						Mesh2::VectorTriangleIndices& 	vTriangleIndices)
{
	Mesh2::VertexIndex v0_index (processVertex(v0, mPositions, mNormals, mTexCoords, hmVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices));
	Mesh2::VertexIndex v1_index (processVertex(v1, mPositions, mNormals, mTexCoords, hmVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices));
	Mesh2::VertexIndex v2_index (processVertex(v2, mPositions, mNormals, mTexCoords, hmVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices));

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
* @oaram hmVertexIndices    A hash map with all the vertex indices per vertex-key
* @param vVertices          A vector with all the vertices
* @param vIndices           A vector with all the indices to the vertices
*
* @return The Mesh
*/
inline void addTriangulatedQuad(const ShapeHelper2::Vertex& v0,
                                const ShapeHelper2::Vertex& v1,
                                const ShapeHelper2::Vertex& v2,
                                const ShapeHelper2::Vertex& v3,
                                MapVec3& 				    mPositions,
                                MapVec3& 				    mNormals,
                                MapVec2& 				    mTexCoords,
                                HashMapVertexIndices& 		hmVertexIndices,
                                Mesh2::VectorPositions& 	vPositions,
                                Mesh2::VectorNormals& 		vNormals,
                                Mesh2::VectorTexCoords& 	vTexCoords,
                                Mesh2::VectorVertexIndices& vVertexIndices,
  							  Mesh2::VectorTriangleIndices& vTriangleIndices)
{
    addTriangle(v0, v1, v2, mPositions, mNormals, mTexCoords, hmVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
    addTriangle(v0, v2, v3, mPositions, mNormals, mTexCoords, hmVertexIndices, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
}



/**
* @brief Terrain Builder function
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
void ShapeHelper2::buildTerrain(std::string&   name,
       						    const JU::f32* height_map,
       						    JU::uint32	   num_rows,
       						    JU::uint32	   num_cols,
								Mesh2&		   mesh)
{
	Mesh2::VectorPositions  	  vPositions (num_rows * num_cols);
	Mesh2::VectorNormals 		  vNormals (num_rows * num_cols);
	Mesh2::VectorTexCoords		  vTexCoords (num_rows * num_cols);
	//Mesh2::VectorVertexIndices    vVertexIndices (num_rows * num_cols);
	Mesh2::VectorVertexIndices    vVertexIndices;
	//Mesh2::VectorTriangleIndices  vTriangleIndices (num_rows * num_cols * 2);
	Mesh2::VectorTriangleIndices  vTriangleIndices;

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    MapVec3 mPositions	(vec3Compare);
    MapVec3 mNormals  	(vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(num_rows * num_cols * 3, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    // Error out if wrong dimensions
    if (num_rows < 2 || num_cols < 2)
    {
    	std::printf("Dimensions of height map (%i, %i) do not allow for mesh creation\n", num_rows, num_cols);
    	exit(EXIT_FAILURE);
    }

    JU::f32 incx = 1.0f / num_cols;
    JU::f32 incy = 1.0f / num_rows;
    JU::f32 x = -0.5f;
    JU::f32 y =  0.5f;
    JU::f32 tx = 0.0f;
    JU::f32 ty = 1.0f;

    // Positions
    glm::vec3 p0, p1, p2, p3;
    // Normal
    glm::vec3 n;
    // Texture Coordinates
    glm::vec2 t0, t1, t2, t3;

    for (JU::uint32 row = 0; row < (num_rows - 1); ++row)
    {
    	x = -0.5f;
    	tx = 0.0f;

    	for (JU::uint32 col = 0; col < (num_cols - 1); ++col)
    	{
            JU::uint32 index = row * num_cols + col;

    		std::printf ("(%i, %i)\n", row, col);

    		// Positions
    		p0.x =        x; p0.y =        y; p0.z = height_map[row * num_cols + col];
    		p1.x =        x; p1.y = y - incy; p1.z = height_map[(row + 1) * num_cols + col];
    		p2.x = x + incx; p2.y = y - incy; p2.z = height_map[(row + 1) * num_cols + (col + 1)];
    		p3.x = x + incx; p3.y =        y; p3.z = height_map[row * num_cols + (col + 1)];
    		// Normal
    		glm::vec3 n(glm::normalize(glm::cross(p1 - p0, p2 - p0)));
    		// Texture Coordinates
    		t0.s = tx;        t0.t = ty;
    		t1.s = tx;        t1.t = ty - incy;
    		t2.s = tx + incx; t2.t = ty - incy;
    		t3.s = tx + incx; t3.t = ty;

    		// DEBUG: START
    		/*
    		debug::print("P0 = ", p0);
    		debug::print("P1 = ", p1);
    		debug::print("P2 = ", p2);
    		debug::print("P3 = ", p3);
			*/
    		// DEBUG: END

    	    Vertex v0(p0, n, t0);
    	    Vertex v1(p1, n, t1);
    	    Vertex v2(p2, n, t2);
    	    Vertex v3(p3, n, t3);

    	    addTriangulatedQuad(v0, v1, v2, v3,
    	    					mPositions, mNormals, mTexCoords, hmVertexIndices,
    	    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

    	    x += incx;
    	    tx += incx;
    	}
    	y -= incy;
    	ty -= incy;
    }

    mesh = Mesh2(name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
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
void ShapeHelper2::buildMesh(Mesh2& mesh, ShapeType shape_type, JU::uint32 num_slices, JU::uint32 num_stacks, JU::f32 radius)
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

        case TORUS:
            ShapeHelper2::buildTorus(shape_name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices, num_slices, num_stacks, radius);
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
    name = std::string("plane");

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    MapVec3 mPositions	(vec3Compare);
    MapVec3 mNormals  	(vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(8, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
}



void ShapeHelper2::buildCube(std::string&  					name,
		   	   	   	   	     Mesh2::VectorPositions&  		vPositions,
		   	   	   	   	     Mesh2::VectorNormals& 			vNormals,
		   	   	   	   	     Mesh2::VectorTexCoords&		vTexCoords,
		   	   	   	   	     Mesh2::VectorVertexIndices& 	vVertexIndices,
							 Mesh2::VectorTriangleIndices&  vTriangleIndices)
{
    name = std::string("cube");

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    MapVec3 mPositions	(vec3Compare);
    MapVec3 mNormals  	(vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(30, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
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
    					mPositions, mNormals, mTexCoords, hmVertexIndices,
    					vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
}



void ShapeHelper2::buildCylinder(std::string&  					name,
		   	   	   	   	   	   	 Mesh2::VectorPositions&  		vPositions,
		   	   	   	   	   	   	 Mesh2::VectorNormals& 			vNormals,
		   	   	   	   	   	   	 Mesh2::VectorTexCoords&		vTexCoords,
		   	   	   	   	   	   	 Mesh2::VectorVertexIndices& 	vVertexIndices,
								 Mesh2::VectorTriangleIndices& 	vTriangleIndices,
                                 JU::uint32  					num_slices)
{
    // CONSTANTS
    const glm::vec3 ORIGIN (0.0f, 0.0f, 0.0f);          // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET (0.5f);                        // Distance from the ORIGIN to the top (or bottom)
    const float RADIUS   (0.5f);                        // RADIUS of the cylinder
    const float DELTA_THETA (2 * M_PI / num_slices);    // Increment of the angle from slice to slice
    const float DELTA_S(1.0F / num_slices);				// Increment of the s texture coordinate from slice to slice

    if (num_slices < 3)
    {
    	std::printf("ShapeHelper2::buildCylinder: minimum number of slices is 3 (%i requested)\n", num_slices);
    	exit(EXIT_FAILURE);
    }

    char buffer[100];
    sprintf(buffer, "cylinder_%i", num_slices);
    name = std::string(buffer);

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    MapVec3 mPositions	(vec3Compare);
    MapVec3 mNormals  	(vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(10 * num_slices, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0, v1, v2, v3;

    // TOP DISK
    const glm::vec3 center_top (0.0f, 0.0f, Z_OFFSET);
    const glm::vec3 top_normal (0.0f, 0.0f, 1.0f);
    float theta = 0.0f;
    for (JU::uint32 slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta); 				JU::f32 y1 = RADIUS * sin(theta);
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, Z_OFFSET);
        glm::vec3 pos2 (x2, y2, Z_OFFSET);

        v0 = Vertex(center_top, // position
                    top_normal, // normal
                    glm::vec2(0.0f + 0.5f,  0.0f + 0.5f));      // texture coordinates
        v1 = Vertex(pos1, // position
                    top_normal, // normal
                    glm::vec2(x1 + 0.5f, y1 + 0.5f));      // texture coordinates
        v2 = Vertex(pos2, // position
                    top_normal, // normal
                    glm::vec2(x2 + 0.5f, y2 + 0.5f));      // texture coordinates
        addTriangle(v0, v1, v2,
        			mPositions, mNormals, mTexCoords, hmVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // BOTTOM DISK
    const glm::vec3 center_bottom (0.0f, 0.0f, -Z_OFFSET);
    const glm::vec3 bottom_normal (0.0f, 0.0f, -1.0f);
    theta = 0.0f;
    for (JU::uint32 slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta); 				JU::f32 y1 = RADIUS * sin(theta);
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, -Z_OFFSET);
        glm::vec3 pos2 (x2, y2, -Z_OFFSET);

        v0 = Vertex(center_bottom, // position
                    bottom_normal, // normal
                    glm::vec2(0.0f + 0.5f, 0.0f + 0.5f));      // texture coordinates
        v1 = Vertex(pos2, // position
        			bottom_normal, // normal
                    glm::vec2(-x2 + 0.5f, y2 + 0.5f));      // texture coordinates
        v2 = Vertex(pos1, // position
        			bottom_normal, // normal
                    glm::vec2(-x1 + 0.5f, y1 + 0.5f));      // texture coordinates
        addTriangle(v0, v1, v2,
        			mPositions, mNormals, mTexCoords, hmVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // SIDE OF THE CYLINDER
    theta = 0.0f;
    float tex_s = 0.0f;
    for (JU::uint32 slice = 0; slice < num_slices; slice++)
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
        			mPositions, mNormals, mTexCoords, hmVertexIndices,
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
	   	   	   	 	 	 	 JU::uint32  					num_slices)
{
    // CONSTANTS
    const glm::vec3 ORIGIN (0.0f, 0.0f, 0.0f);          // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET (0.5f);                        // Distance from the ORIGIN to the top (or bottom)
    const float RADIUS   (0.5f);                        // RADIUS of the cylinder
    const float DELTA_THETA (2 * M_PI / num_slices);    // Increment of the angle from slice to slice
    const float DELTA_S(1.0F / num_slices);             // Increment of the s texture coordinate from slice to slice

    if (num_slices < 3)
    {
    	std::printf("ShapeHelper2::buildCone: minimum number of slices is 3 (%i requested)\n", num_slices);
    	exit(EXIT_FAILURE);
    }

    char buffer[100];
    sprintf(buffer, "cone_%i", num_slices);
    name = std::string(buffer);

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;                      // Vertex data
    MapVec3 mPositions (vec3Compare);
    MapVec3 mNormals   (vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(10 * num_slices, vertexIndicesHash);       // Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0, v1, v2, v3;

    // BOTTOM DISK
    const glm::vec3 center_bottom (0.0f, 0.0f, -Z_OFFSET);
    const glm::vec3 bottom_normal (0.0f, 0.0f, -1.0f);
    float theta = 0.0f;
    for (JU::uint32 slice = 0; slice < num_slices; slice++)
    {
        JU::f32 x1 = RADIUS * cos(theta);               JU::f32 y1 = RADIUS * sin(theta);
        JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, -Z_OFFSET);
        glm::vec3 pos2 (x2, y2, -Z_OFFSET);

        v0 = Vertex(center_bottom, // position
                    bottom_normal, // normal
                    glm::vec2(0.0f + 0.5f, 0.0f + 0.5f));      // texture coordinates
        v1 = Vertex(pos2, // position
                    bottom_normal, // normal
                    glm::vec2(-x2 + 0.5f, y2 + 0.5f));      // texture coordinates
        v2 = Vertex(pos1, // position
                    bottom_normal, // normal
                    glm::vec2(-x1 + 0.5f, y1 + 0.5f));      // texture coordinates
        addTriangle(v0, v1, v2,
                    mPositions, mNormals, mTexCoords, hmVertexIndices,
                    vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // SIDE OF THE CONE
    const glm::vec3 appex_position (0.0f, 0.0f, +Z_OFFSET);
    theta = 0.0f;
    float tex_s = 0.0f;
    for (JU::uint32 slice = 0; slice < num_slices; slice++)
    {
        JU::f32 x1 = RADIUS * cos(theta);               JU::f32 y1 = RADIUS * sin(theta);
        JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA); JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA);
        glm::vec3 pos1 (x1, y1, -Z_OFFSET);
        glm::vec3 pos2 (x2, y2, -Z_OFFSET);
        glm::vec3 norm1 (glm::normalize(pos1 - center_bottom));
        norm1[2] = 0.5f;
        norm1 = glm::normalize(norm1);
        glm::vec3 norm2 (glm::normalize(pos2 - center_bottom));
        norm2[2] = 0.5f;
        norm2 = glm::normalize(norm2);
        glm::vec3 appex_norm = glm::normalize(0.5f * (norm1 + norm2));
        JU::f32 s1 = slice * DELTA_S;
        JU::f32 s2 = (slice + 1) * DELTA_S;

        v0 = Vertex(appex_position, // position
                    appex_norm, // normal
                    glm::vec2((s1 + s2) * 0.5f, 1.0f));      // texture coordinates
        v1 = Vertex(pos1, // position
                    norm1, // normal
                    glm::vec2(s1, 0.0f));      // texture coordinates
        v2 = Vertex(pos2, // position
                    norm2, // normal
                    glm::vec2(s2, 0.0f));      // texture coordinates
        addTriangle(v0, v1, v2,
                    mPositions, mNormals, mTexCoords, hmVertexIndices,
                    vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
        tex_s += DELTA_S;
    }
}



void ShapeHelper2::buildSphere(std::string&  				 name,
	   	   	   	 	 	 	   Mesh2::VectorPositions&  	 vPositions,
	   	   	   	 	 	 	   Mesh2::VectorNormals& 		 vNormals,
	   	   	   	 	 	 	   Mesh2::VectorTexCoords&		 vTexCoords,
	   	   	   	 	 	 	   Mesh2::VectorVertexIndices& 	 vVertexIndices,
	   	   	   	 	 	 	   Mesh2::VectorTriangleIndices& vTriangleIndices,
	   	   	   	 	 	 	   JU::uint32  				 	 num_slices,
                               JU::uint32  				 	 num_stacks)
{
    // CONSTANTS
    const glm::vec3 ORIGIN (0.0f, 0.0f, 0.0f);          // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET (0.5f);                        // Distance from the ORIGIN to the top (or bottom)
    const float RADIUS   (0.5f);                        // RADIUS of the cylinder
    const float DELTA_THETA (2 * M_PI / num_slices);    // Increment of the angle from slice to slice
    const float DELTA_PHI (M_PI / num_stacks);          // Angle increments for each stack
    const float DELTA_S(1.0F / num_slices);				// Increment of the s texture coordinate from slice to slice
    const float DELTA_T(1.0F / num_stacks);				// Increment of the s texture coordinate from slice to slice

    if (num_slices < 3)
    {
    	std::printf("ShapeHelper2::buildSphere: minimum number of slices/stacks is 3 (%i/%i requested)\n", num_slices, num_stacks);
    	exit(EXIT_FAILURE);
    }

    char buffer[100];
    sprintf(buffer, "sphere_%i_%i", num_slices, num_stacks);
    name = std::string(buffer);

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    MapVec3 mPositions	(vec3Compare);
    MapVec3 mNormals  	(vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(10 * num_slices, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0, v1, v2, v3;

    // TOP DISK
    const glm::vec3 center_top (0.0f, 0.0f, Z_OFFSET);
    const glm::vec3 top_normal (0.0f, 0.0f, 1.0f);
    float theta = 0.0f;
    float phi = DELTA_PHI;
    float sin_phi = sin(phi);
    float z = RADIUS * cos(phi);
    JU::f32 t = 1.0f - DELTA_T;
    for (JU::uint16 slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta) * sin_phi; 				JU::f32 y1 = RADIUS * sin(theta) * sin_phi;
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA) * sin_phi; 	JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA) * sin_phi;
        glm::vec3 pos1 (x1, y1, z);
        glm::vec3 pos2 (x2, y2, z);
        glm::vec3 norm1 (glm::normalize(pos1 - ORIGIN));
        glm::vec3 norm2 (glm::normalize(pos2 - ORIGIN));
        JU::f32 s1 = slice * DELTA_S;
        JU::f32 s2 = (slice + 1) * DELTA_S;


        v0 = Vertex(center_top, // position
                    top_normal, // normal
                    glm::vec2(0.5f * (s1 + s2), 1.0f));      // texture coordinates
        v1 = Vertex(pos1, // position
                    norm1, // normal
                    glm::vec2(s1, t));      // texture coordinates
        v2 = Vertex(pos2, // position
                    norm2, // normal
                    glm::vec2(s2, t));      // texture coordinates
        addTriangle(v0, v1, v2,
        			mPositions, mNormals, mTexCoords, hmVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // BOTTOM DISK
    const glm::vec3 center_bottom (0.0f, 0.0f, -Z_OFFSET);
    const glm::vec3 bottom_normal (0.0f, 0.0f, -1.0f);
    theta = 0.0f;
    phi = M_PI - DELTA_PHI;
    sin_phi = sin(phi);
    z = RADIUS * cos(phi);
    t = DELTA_T;
    for (JU::uint16 slice = 0; slice < num_slices; slice++)
    {
    	JU::f32 x1 = RADIUS * cos(theta) * sin_phi; 				JU::f32 y1 = RADIUS * sin(theta) * sin_phi;
    	JU::f32 x2 = RADIUS * cos(theta + DELTA_THETA) * sin_phi; 	JU::f32 y2 = RADIUS * sin(theta + DELTA_THETA) * sin_phi;
        glm::vec3 pos1 (x1, y1, z);
        glm::vec3 pos2 (x2, y2, z);
        glm::vec3 norm1 (glm::normalize(pos1 - ORIGIN));
        glm::vec3 norm2 (glm::normalize(pos2 - ORIGIN));
        JU::f32 s1 = slice * DELTA_S;
        JU::f32 s2 = (slice + 1) * DELTA_S;


        v0 = Vertex(pos2, // position
                    norm2, // normal
                    glm::vec2(s2, t));      // texture coordinates
        v1 = Vertex(pos1, // position
                    norm1, // normal
                    glm::vec2(s1, t));      // texture coordinates
        v2 = Vertex(center_bottom, // position
                    bottom_normal, // normal
                    glm::vec2(0.5f * (s1 + s2), 0.0f));      // texture coordinates
        addTriangle(v0, v1, v2,
        			mPositions, mNormals, mTexCoords, hmVertexIndices,
        			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);

        theta += DELTA_THETA;
    }

    // EVERYTHING BUT TOP AND BOTTOM DISKS
    for (JU::uint32 stack = 1; stack < (num_stacks - 1); ++stack)
    {
    	JU::f32 phi1 	 = stack * DELTA_PHI;
    	JU::f32 phi2 	 = (stack + 1) * DELTA_PHI;
    	JU::f32 sin_phi1 = sin(phi1);
    	JU::f32 sin_phi2 = sin(phi2);
		JU::f32 z1 		 = RADIUS * cos(phi1);
		JU::f32 z2 		 = RADIUS * cos(phi2);
		JU::f32 t1		 = 1.0f - (stack * DELTA_T);
		JU::f32 t2		 = 1.0f - ((stack + 1) * DELTA_T);

        for (JU::uint32 slice = 0; slice < num_slices; slice++)
        {
        	JU::f32 theta1 = slice * DELTA_THETA;
        	JU::f32 theta2 = (slice + 1) * DELTA_THETA;
        	JU::f32 cos_theta1 = cos(theta1);
        	JU::f32 cos_theta2 = cos(theta2);
        	JU::f32 sin_theta1 = sin(theta1);
        	JU::f32 sin_theta2 = sin(theta2);
        	JU::f32 x0 = RADIUS * cos_theta1 * sin_phi1; 	JU::f32 y0 = RADIUS * sin_theta1 * sin_phi1;
        	JU::f32 x1 = RADIUS * cos_theta1 * sin_phi2; 	JU::f32 y1 = RADIUS * sin_theta1 * sin_phi2;
        	JU::f32 x2 = RADIUS * cos_theta2 * sin_phi2; 	JU::f32 y2 = RADIUS * sin_theta2 * sin_phi2;
        	JU::f32 x3 = RADIUS * cos_theta2 * sin_phi1; 	JU::f32 y3 = RADIUS * sin_theta2 * sin_phi1;
        	glm::vec3 pos0 (x0, y0, z1);
        	glm::vec3 pos1 (x1, y1, z2);
        	glm::vec3 pos2 (x2, y2, z2);
        	glm::vec3 pos3 (x3, y3, z1);
        	glm::vec3 norm0 (glm::normalize(pos0 - ORIGIN));
        	glm::vec3 norm1 (glm::normalize(pos1 - ORIGIN));
        	glm::vec3 norm2 (glm::normalize(pos2 - ORIGIN));
        	glm::vec3 norm3 (glm::normalize(pos3 - ORIGIN));
    		JU::f32 s1 = slice * DELTA_S;
    		JU::f32 s2 = (slice + 1) * DELTA_S;

            v0 = Vertex(pos0, // position
                        norm0, // normal
                        glm::vec2(s1, t1));      // texture coordinates
            v1 = Vertex(pos1, // position
            			norm1, // normal
                        glm::vec2(s1, t2));      // texture coordinates
            v2 = Vertex(pos2, // position
            			norm2, // normal
                        glm::vec2(s2, t2));      // texture coordinates
            v3 = Vertex(pos3, // position
            			norm3, // normal
                        glm::vec2(s2, t1));      // texture coordinates
            addTriangulatedQuad(v0, v1, v2, v3,
            			mPositions, mNormals, mTexCoords, hmVertexIndices,
            			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
        }
    }
}


void ShapeHelper2::buildTorus(std::string&  				 name,
	   	   	   	 	 	 	   Mesh2::VectorPositions&  	 vPositions,
	   	   	   	 	 	 	   Mesh2::VectorNormals& 		 vNormals,
	   	   	   	 	 	 	   Mesh2::VectorTexCoords&		 vTexCoords,
	   	   	   	 	 	 	   Mesh2::VectorVertexIndices& 	 vVertexIndices,
	   	   	   	 	 	 	   Mesh2::VectorTriangleIndices& vTriangleIndices,
	   	   	   	 	 	 	   JU::uint32  				 	 num_slices1,
                               JU::uint32  				 	 num_slices2,
                               JU::f32						 radius)
{
    // CONSTANTS
    const glm::vec3 ORIGIN 		(0.0f, 0.0f, 0.0f);         // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET 		(0.5f);                     // Distance from the ORIGIN to the top (or bottom)
    const float TUBE_RADIUS		(radius);					// Radius of the tube
    const float TORUS_RADIUS  	(0.5f - TUBE_RADIUS);       // Distance from the origin (center of the torus) to the center of a section
    const float DELTA_THETA 	(2 * M_PI / num_slices1);  	// Increment of the angle from slice to slice
    const float DELTA_PHI 		(2 * M_PI / num_slices2);      	// Increment of the angle within a slice
    const float DELTA_S			(1.0F / num_slices1);		// Increment of the s texture coordinate
    const float DELTA_T			(1.0F / num_slices2);		// Increment of the s texture coordinate

    if (num_slices1 < 3 || num_slices2 < 3)
    {
    	std::printf("ShapeHelper2::buildTorus: minimum number of slices/stacks is 3 (%i/%i requested)\n", num_slices1, num_slices2);
    	exit(EXIT_FAILURE);
    }

    if (radius > 0.5f)
    {
    	std::printf("ShapeHelper2::buildTorus: max tube radius is 0.5f (%f requested)\n", radius);
    	exit(EXIT_FAILURE);
    }

    char buffer[100];
    sprintf(buffer, "torus_%i_%i_%.2f", num_slices1, num_slices2, radius);
    name = std::string(buffer);

    vPositions.clear();
    vNormals.clear();
    vTexCoords.clear();
    vVertexIndices.clear();

    Vertex vertex;						// Vertex data
    MapVec3 mPositions	(vec3Compare);
    MapVec3 mNormals  	(vec3Compare);
    MapVec2 mTexCoords (vec2Compare);
    HashMapVertexIndices hmVertexIndices(10 * num_slices1 * num_slices2, vertexIndicesHash);		// Hash map to keep track of uniqueness of vertices and their indices

    Vertex v0, v1, v2, v3;

    // For each SLICE of the TORUS
    for (JU::uint32 slice1 = 0; slice1 < num_slices1; ++slice1)
    {
    	float theta1 = DELTA_THETA * slice1;
    	float theta2 = DELTA_THETA * (slice1 + 1);
    	float cos_theta1 = cos(theta1);
    	float cos_theta2 = cos(theta2);
    	float sin_theta1 = sin(theta1);
    	float sin_theta2 = sin(theta2);
    	glm::vec3 tube_center1 (TORUS_RADIUS * cos_theta1, TORUS_RADIUS * sin_theta1, 0.0f);
    	glm::vec3 tube_center2 (TORUS_RADIUS * cos_theta2, TORUS_RADIUS * sin_theta2, 0.0f);

    	// For each slice of the TUBE
    	for (JU::uint32 slice2 = 0; slice2 < num_slices2; ++slice2)
    	{
    		float phi1 = DELTA_PHI * (slice2 + 1);
    		float phi2 = DELTA_PHI * slice2;
        	float term_phi1 = TORUS_RADIUS + TUBE_RADIUS * cos(phi1);
        	float term_phi2 = TORUS_RADIUS + TUBE_RADIUS * cos(phi2);
        	float sin_phi1 = sin(phi1);
        	float sin_phi2 = sin(phi2);

        	// POSITIONS
        	glm::vec3 pos0 (term_phi1 * cos_theta1, term_phi1 * sin_theta1, TUBE_RADIUS * sin_phi1);
        	glm::vec3 pos1 (term_phi2 * cos_theta1, term_phi2 * sin_theta1, TUBE_RADIUS * sin_phi2);
        	glm::vec3 pos2 (term_phi2 * cos_theta2, term_phi2 * sin_theta2, TUBE_RADIUS * sin_phi2);
        	glm::vec3 pos3 (term_phi1 * cos_theta2, term_phi1 * sin_theta2, TUBE_RADIUS * sin_phi1);
        	// NORMALS
        	glm::vec3 norm0 (glm::normalize(pos0 - tube_center1));
        	glm::vec3 norm1 (glm::normalize(pos1 - tube_center1));
        	glm::vec3 norm2 (glm::normalize(pos2 - tube_center2));
        	glm::vec3 norm3 (glm::normalize(pos3 - tube_center2));
        	// TEXTURE COORDINATES
        	float s1 = DELTA_S * slice1;
        	float s2 = DELTA_S * (slice1 + 1);
			float t1 = DELTA_T * (slice2 + 1);
			float t2 = DELTA_T * slice2;

            v0 = Vertex(pos0, // position
                        norm0, // normal
                        glm::vec2(s1, t1));      // texture coordinates
            v1 = Vertex(pos1, // position
            			norm1, // normal
                        glm::vec2(s1, t2));      // texture coordinates
            v2 = Vertex(pos2, // position
            			norm2, // normal
                        glm::vec2(s2, t2));      // texture coordinates
            v3 = Vertex(pos3, // position
            			norm3, // normal
                        glm::vec2(s2, t1));      // texture coordinates
            addTriangulatedQuad(v0, v1, v2, v3,
            			mPositions, mNormals, mTexCoords, hmVertexIndices,
            			vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
    	}
    }
}


