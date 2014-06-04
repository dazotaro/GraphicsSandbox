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

