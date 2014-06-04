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
#include <glm/glm.hpp>		// glm::vec2, glm::vec3

// Local includes
#include "Mesh2.hpp"		// Mesh2


class ShapeHelper2
{
	public:

		struct Vertex
		{
			glm::vec3 position_;
			glm::vec3 normal_;
			glm::vec2 tex_coords_;

			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coords)
				: position_(position), normal_(normal), tex_coords_(tex_coords) {}

			Vertex(JU::f32 px = 0.0f, JU::f32 py = 0.0f, JU::f32 pz = 0.0f,
				   JU::f32 nx = 0.0f, JU::f32 ny = 0.0f, JU::f32 nz = 0.0f,
				   JU::f32 s = 0.0f, JU::f32 t = 0.0f)
			{
				position_[0] = px; 	position_[1] = py; 	position_[2] = pz;
				normal_[0] = nx; 	normal_[1] = ny; 	normal_[2] = nz;
				tex_coords_[0] = s; 	tex_coords_[1] = t;
			}

			bool operator==(const Vertex& rhs) const
			{
				if (position_[0] != rhs.position_[0] || position_[1] != rhs.position_[1] || position_[2] != rhs.position_[2]
					|| normal_[0] != rhs.normal_[0] || normal_[1] != rhs.normal_[1] || normal_[2] != rhs.normal_[2]
					|| tex_coords_[0] != rhs.tex_coords_[0] || tex_coords_[1] != rhs.tex_coords_[1])
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

		// TYPEDEFS and ENUMs
		typedef std::vector<JU::uint32>         IndexVector;
		typedef IndexVector::const_iterator     IndexVectorConstIter;
		typedef std::vector<Vertex>             VertexVector;
		typedef VertexVector::const_iterator    VertexVectorConstIter;

	public:
		// BUILDER FUNCTIONS: they build the desired shape
		//-------------------------------------------------
		static void buildMesh(Mesh2& mesh, ShapeType shape_type, unsigned int num_slices = 10, unsigned int num_stacks = 10);

		static void buildPlane(std::string&  				 name,
							   Mesh2::VectorPositions&  	 vPositions,
							   Mesh2::VectorNormals& 		 vNormals,
							   Mesh2::VectorTexCoords&		 vTexCoords,
							   Mesh2::VectorVertexIndices& 	 vVertexIndices,
							   Mesh2::VectorTriangleIndices& vTriangleIndices);

		static void buildCube(std::string&  				name,
				   	   	   	  Mesh2::VectorPositions&  		vPositions,
				   	   	   	  Mesh2::VectorNormals& 		vNormals,
				   	   	   	  Mesh2::VectorTexCoords&		vTexCoords,
				   	   	   	  Mesh2::VectorVertexIndices& 	vVertexIndices,
							  Mesh2::VectorTriangleIndices& vTriangleIndices);

		static void buildCylinder(std::string&  				name,
				   	   	   	   	  Mesh2::VectorPositions&  		vPositions,
				   	   	   	   	  Mesh2::VectorNormals& 		vNormals,
				   	   	   	   	  Mesh2::VectorTexCoords&		vTexCoords,
				   	   	   	   	  Mesh2::VectorVertexIndices& 	vVertexIndices,
								  Mesh2::VectorTriangleIndices& vTriangleIndices,
								  unsigned int  				num_slices);

		static void buildCone(std::string&  				name,
				   	   	   	  Mesh2::VectorPositions&  		vPositions,
				   	   	   	  Mesh2::VectorNormals& 		vNormals,
				   	   	   	  Mesh2::VectorTexCoords&		vTexCoords,
				   	   	   	  Mesh2::VectorVertexIndices& 	vVertexIndices,
							  Mesh2::VectorTriangleIndices& vTriangleIndices,
							  unsigned int  				num_slices);

		static void buildSphere(std::string&  					name,
				   	   	   	    Mesh2::VectorPositions&  		vPositions,
				   	   	   	    Mesh2::VectorNormals& 			vNormals,
				   	   	   	    Mesh2::VectorTexCoords&			vTexCoords,
				   	   	   	    Mesh2::VectorVertexIndices& 	vVertexIndices,
								Mesh2::VectorTriangleIndices& 	vTriangleIndices,
								unsigned int  					num_slices,
								unsigned int  					num_stacks);
};


#endif /* SHAPEHELPER2_HPP_ */
