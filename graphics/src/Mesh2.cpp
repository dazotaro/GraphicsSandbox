/*
 * Mesh22.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

// Local include
#include "Mesh2.hpp"

// Global include
#include <iostream>     // std::cout

/**
* @brief Default Constructor
*/
Mesh2::Mesh2()
{
}



/**
* @brief Non-Default Constructor
*
* @param name Id of the Mesh2
* @param positions Vector with all vertex positions
* @param normals Vector with all vertex normals
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh2
*/
Mesh2::Mesh2(const std::string&				name,
			 const VectorPositions&			vPositions,
			 const VectorNormals&			vNormals,
			 const VectorTexCoords&			vTexCoords,
			 const VectorVertexIndices&		vVertexIndices,
			 const VectorTriangleIndices&	vTriangleIndices)
	: name_(name),
	  vPositions_(vPositions),
	  vNormals_(vNormals),
	  vTexCoords_(vTexCoords),
	  vVertexIndices_(vVertexIndices),
	  vTriangleIndices_(vTriangleIndices)
{
}

Mesh2::~Mesh2()
{
    // TODO Auto-generated destructor stub
}

const Mesh2::VectorVertexIndices& Mesh2::getVertexIndices() const
{
    return vVertexIndices_;
}

const Mesh2::VectorTriangleIndices& Mesh2::getTriangleIndices() const
{
    return vTriangleIndices_;
}

const std::string& Mesh2::getName() const
{
    return name_;
}

const Mesh2::VectorNormals& Mesh2::getNormals() const
{
    return vNormals_;
}

const Mesh2::VectorPositions& Mesh2::getPositions() const
{
    return vPositions_;
}

const Mesh2::VectorTexCoords& Mesh2::getTexCoords() const
{
    return vTexCoords_;
}

const Mesh2::VectorTangents& Mesh2::getTangents() const
{
    return vTangents_;
}

void Mesh2::export2OBJ(const char *filename) const
{
    FILE *file = fopen(filename, "w");

    if (!file)
    {
        printf("Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Print the header
    fprintf(file, "# CS300 Graphics::Mesh2 to obj exporter\n");

    // POSITIONS
    for (VectorPositionsConstIter iter = vPositions_.begin(); iter != vPositions_.end(); ++iter)
    {
        fprintf(file, "v %f %f %f\n", iter->x, iter->y, iter->z);
    }

    // TEXTURE COORDINATES
    for (VectorTexCoordsConstIter iter = vTexCoords_.begin(); iter != vTexCoords_.end(); ++iter)
    {
        fprintf(file, "vt %f %f\n", iter->s, iter->t);
    }

    // NORMALS
    for (VectorNormalsConstIter iter = vNormals_.begin(); iter != vNormals_.end(); ++iter)
    {
        fprintf(file, "vn %f %f %f\n", iter->x, iter->y, iter->z);
    }

    // Use material
    fprintf(file, "usemtl (null)\n");

    // Smoothing
    fprintf(file, "s off\n");

    // FACES
    VectorTriangleIndicesConstIter triangle_iter;
    for (triangle_iter = vTriangleIndices_.begin(); triangle_iter != vTriangleIndices_.end(); ++triangle_iter)
    {
        fprintf(file, "f ");

		fprintf (file, "  %i/%i/%i", vVertexIndices_[triangle_iter->v0_].position_ + 1,
									 vVertexIndices_[triangle_iter->v0_].tex_ + 1,
									 vVertexIndices_[triangle_iter->v0_].normal_ + 1);

		fprintf (file, "  %i/%i/%i", vVertexIndices_[triangle_iter->v1_].position_ + 1,
									 vVertexIndices_[triangle_iter->v1_].tex_ + 1,
									 vVertexIndices_[triangle_iter->v1_].normal_ + 1);

		fprintf (file, "  %i/%i/%i", vVertexIndices_[triangle_iter->v2_].position_ + 1,
									 vVertexIndices_[triangle_iter->v2_].tex_ + 1,
									 vVertexIndices_[triangle_iter->v2_].normal_ + 1);

        fprintf (file, " \n");
    }

    fclose(file);
}

void Mesh2::computeTangents()
{
	JU::uint32 num_vertices (vVertexIndices_.size());
	vTangents_.resize(num_vertices);

    // Per-Vertex vectors
    std::vector<glm::vec3> tan (num_vertices, glm::vec3(0.0f, 0.0f, 0.0f));
    std::vector<glm::vec3> bit (num_vertices, glm::vec3(0.0f, 0.0f, 0.0f));

    // PER-FACE: compute tangent and bitangent and accumulate them
	for (VectorTriangleIndicesConstIter triangle_iter = vTriangleIndices_.begin(); triangle_iter != vTriangleIndices_.end(); ++triangle_iter)
	{
		JU::uint32& p0 (vVertexIndices_[triangle_iter->v0_].position_);
		JU::uint32& p1 (vVertexIndices_[triangle_iter->v1_].position_);
		JU::uint32& p2 (vVertexIndices_[triangle_iter->v2_].position_);

		JU::uint32& t0 (vVertexIndices_[triangle_iter->v0_].tex_);
		JU::uint32& t1 (vVertexIndices_[triangle_iter->v1_].tex_);
		JU::uint32& t2 (vVertexIndices_[triangle_iter->v2_].tex_);

		// Compute the normal of the face
        glm::vec3 vec1 (glm::normalize(vPositions_[p1] - vPositions_[p0]));
        glm::vec3 vec2 (glm::normalize(vPositions_[p2] - vPositions_[p0]));

        glm::vec2 tex1 (vTexCoords_[t1] -vTexCoords_[t0]);
        glm::vec2 tex2 (vTexCoords_[t2] -vTexCoords_[t0]);

        float factor = 1 / (tex1.s*tex2.t - tex2.s*tex1.t);

        /* REDO these next two lines. The program is crashing when the factor is inf/nan */
        if (std::isinf(factor) || std::isnan(factor))
        {
        	std::printf("nan or inf value return in Mesh2::computeTangents\n");
        	exit(EXIT_FAILURE);
        }

        glm::vec3 tmp_tan (factor * ( tex2.t * vec1.x - tex1.t * vec2.x),
                           factor * ( tex2.t * vec1.y - tex1.t * vec2.y),
                           factor * ( tex2.t * vec1.z - tex1.t * vec2.z));
        glm::vec3 tmp_bit (factor * (-tex2.s * vec1.x + tex1.s * vec2.x),
                           factor * (-tex2.s * vec1.y + tex1.s * vec2.y),
                           factor * (-tex2.s * vec1.z + tex1.s * vec2.z));

        // Tangents and bitangents are given the same indices as texture coordinates
		tan[t0] += tmp_tan;
        tan[t1] += tmp_tan;
        tan[t2] += tmp_tan;
        bit[t0] += tmp_bit;
        bit[t1] += tmp_bit;
        bit[t2] += tmp_bit;
	}

	// PER-VERTEX
	for (JU::uint32 index = 0; index < num_vertices; ++index)
	{
		JU::uint32 normal_index (vVertexIndices_[index].normal_);
		JU::uint32 tan_index 	(vVertexIndices_[index].tex_);		// Same as texture coordinates

		glm::vec3& normal = vNormals_[normal_index];

		// Gram-Schmidt orthogonalize
        glm::vec3 tangent (glm::normalize(tan[tan_index] - (glm::dot(normal, tan[tan_index]) * normal)));

        // Calculate handedness}
        float w = (glm::dot(glm::cross(normal, tan[tan_index]), bit[tan_index])) < 0.0f ? -1.0f : 1.0f;        vTangents_[tan_index] = glm::vec4(tangent, w);
	}
}