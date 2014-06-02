/*
 * ShapeHelper2.hpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

#ifndef SHAPEHELPER2_HPP_
#define SHAPEHELPER2_HPP_

// Global includes
#include <vector>			// std::vector
#include <JU/Defs.hpp>		// Custom data types
#include <string>			// std::string

// Forward declarations
class Mesh2;


class ShapeHelper2
{
	public:

		struct Vertex
		{
			JU::f32 position[3];
			JU::f32 normal[3];
			JU::f32 tex_coords[2];

			Vertex(JU::f32 px = 0.0f, JU::f32 py = 0.0f, JU::f32 pz = 0.0f,
				   JU::f32 nx = 0.0f, JU::f32 ny = 0.0f, JU::f32 nz = 0.0f,
				   JU::f32 s = 0.0f, JU::f32 t = 0.0f)
			{
				position[0] = px; 	position[1] = py; 	position[2] = pz;
				normal[0] = nx; 	normal[1] = ny; 	normal[2] = nz;
				tex_coords[0] = s; 	tex_coords[1] = t;
			}

			bool operator==(const Vertex& rhs) const
			{
				if (position[0] != rhs.position[0] || position[1] != rhs.position[1] || position[2] != rhs.position[2]
					|| normal[0] != rhs.normal[0] || normal[1] != rhs.normal[1] || normal[2] != rhs.normal[2]
					|| tex_coords[0] != rhs.tex_coords[0] || tex_coords[1] != rhs.tex_coords[1])
					return false;

				return true;
			}
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

	private:

		// TYPEDEFS and ENUMs
		typedef std::vector<JU::uint32>         IndexVector;
		typedef IndexVector::const_iterator     IndexVectorConstIter;
		typedef std::vector<Vertex>             VertexVector;
		typedef VertexVector::const_iterator    VertexVectorConstIter;

	public:
		// BUILDER FUNCTIONS: they build the desired shape
		//-------------------------------------------------
		static void buildMesh(Mesh2& mesh, ShapeType shape_type, unsigned int num_slices = 10, unsigned int num_stacks = 10);

		static void buildPlane(std::string&  name,
							   IndexVector&  vIndices,
							   VertexVector& vVertices);

		static void buildCube(std::string&  name,
							  IndexVector&  vIndices,
							  VertexVector& vVertices);

		static void buildCylinder(std::string&  name,
								  IndexVector&  vIndices,
								  VertexVector& vVertices,
								  unsigned int  num_slices);

		static void buildCone(std::string&  name,
							  IndexVector&  vIndices,
							  VertexVector& vVertices,
							  unsigned int  num_slices);

		static void buildSphere(std::string&  name,
								IndexVector&  vIndices,
								VertexVector& vVertices,
								unsigned int  num_slices,
								unsigned int  num_stacks);
};


#endif /* SHAPEHELPER2_HPP_ */
