/*
 * Mesh2.hpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

#ifndef MESH2_HPP_
#define MESH2_HPP_

// Global includes
#include <glm/glm.hpp>      // glm::vec3, glm::vec2
#include <vector>           // std::vector
#include <string>           // std::string
#include <JU/core/Defs.hpp>		// uint32



class Mesh2
{
	public:

		typedef JU::uint16 VertexIndex;

		struct VertexIndices
		{
			VertexIndices (JU::uint32 position = 0, JU::uint32 normal = 0, JU::uint32 tex = 0)
				: position_(position), normal_(normal), tex_(tex) {}

			JU::uint32 position_;
			JU::uint32 normal_;
			JU::uint32 tex_;

			bool operator==(const VertexIndices& rhs) const
			{
				if (position_ != rhs.position_ || normal_ != rhs.normal_ || tex_ != rhs.tex_)
					return false;

				return true;
			}
		};

		struct TriangleIndices
		{
	            TriangleIndices (VertexIndex v0 = 0, VertexIndex v1 = 0, VertexIndex v2 = 0)
                : v0_(v0), v1_(v1), v2_(v2) {}

			VertexIndex v0_;
			VertexIndex v1_;
			VertexIndex v2_;
		};

		// TYPEDEFS
		typedef std::vector<glm::vec3> VectorPositions;
		typedef VectorPositions::const_iterator VectorPositionsConstIter;
		typedef std::vector<glm::vec3> VectorNormals;
		typedef VectorNormals::const_iterator VectorNormalsConstIter;
		typedef std::vector<glm::vec2> VectorTexCoords;
		typedef VectorTexCoords::const_iterator VectorTexCoordsConstIter;
		typedef std::vector<glm::vec4> VectorTangents;
		typedef VectorTangents::const_iterator VectorTangentsConstIter;
		typedef std::vector<VertexIndices> VectorVertexIndices;
		typedef VectorVertexIndices::const_iterator VectorVertexIndicesConstIter;
		typedef std::vector<TriangleIndices> VectorTriangleIndices;
		typedef VectorTriangleIndices::const_iterator VectorTriangleIndicesConstIter;

		Mesh2();
		Mesh2(const std::string&			name,
			  const VectorPositions&		vPositions,
			  const VectorNormals&			vNormals,
			  const VectorTexCoords&		vTexCoords,
			  const VectorVertexIndices&	vVertexIndices,
			  const VectorTriangleIndices& 	vTriangleIndices);

		virtual ~Mesh2();

		// UTILITY FUNCTIONS
		void computeTangents();

		// GETTERS
		const VectorVertexIndices& 		getVertexIndices() const;
		const VectorTriangleIndices& 	getTriangleIndices() const;
		const std::string& 				getName() const;
		const VectorNormals& 			getNormals() const;
		const VectorPositions& 			getPositions() const;
		const VectorTexCoords& 			getTexCoords() const;
		const VectorTangents&			getTangents() const;

		// EXPORT AND OUTPUT FUNCTIONS
		void exportOBJ(void) const;
		void export2OBJ(const char *filename) const;
		friend std::ostream& operator<<(std::ostream &out, const Mesh2 &rhs);

	private:

		std::string     	  name_;          	//!< ID of the Mesh
		VectorPositions		  vPositions_;    	//!< Vector of vertex coordinates
		VectorNormals      	  vNormals_;      	//!< Vector of vertex normals
		VectorTexCoords    	  vTexCoords_;    	//!< Vector of vertex texture coordinates
		VectorTangents		  vTangents_;		//!< Vector of vertex tangents
		VectorVertexIndices   vVertexIndices_;  //!< Vector of face indices
		VectorTriangleIndices vTriangleIndices_;//!< Vector of triangle indices
};

#endif /* MESH2_HPP_ */
