/*
 * Mesh2.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: jusabiaga
 */

#include "Mesh2.hpp"



Mesh2::Mesh2(const std::string& name, const IndexVector& vIndices, const VertexVector& vVertices)
            : name_(name), vIndices_(vIndices), vVertices_(vVertices)
{
}



Mesh2::~Mesh2()
{
    // TODO Auto-generated destructor stub
}



const std::string& Mesh2::getName() const
{
    return name_;
}

void Mesh2::getIndices(IndexVector& vIndices) const
{
    vIndices = vIndices_;
}



void Mesh2::getPositions(std::vector<glm::vec3>& vPositions) const
{
    // Set the capacity of the vector and reset
    vPositions.clear();
    vPositions.resize(vVertices_.size());

    for (VertexVectorConstIter iter = vVertices_.begin(); iter != vVertices_.end(); ++iter)
    {
        vPositions.push_back(iter->position);
    }
}



void Mesh2::getNormals(std::vector<glm::vec3>& vNormals) const
{
    // Set the capacity of the vector and reset
    vNormals.clear();
    vNormals.resize(vVertices_.size());

    for (VertexVectorConstIter iter = vVertices_.begin(); iter != vVertices_.end(); ++iter)
    {
        vNormals.push_back(iter->position);
    }
}



void Mesh2::getTexCoords(std::vector<glm::vec2>& vTexCoords) const
{
    // Set the capacity of the vector and reset
    vTexCoords.clear();
    vTexCoords.resize(vVertices_.size());

    for (VertexVectorConstIter iter = vVertices_.begin(); iter != vVertices_.end(); ++iter)
    {
        vTexCoords.push_back(iter->position);
    }
}

