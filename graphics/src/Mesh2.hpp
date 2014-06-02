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



class Mesh2
{
    // TYPEDEFS and Structure Definitions
    public:
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 tex_coords;

            bool operator==(const Vertex& rhs) const
            {
                if (position != rhs.position || normal != rhs.normal || tex_coords != rhs.tex_coords)
                    return false;

                return true;
            }
        };

        typedef std::vector<JU::uint32>         IndexVector;
        typedef IndexVector::const_iterator     IndexListConstIter;
        typedef std::vector<Vertex>             VertexVector;
        typedef VertexVector::const_iterator    VertexVectorConstIter;

    // Member functions
    public:
        Mesh2(const std::string& name, const IndexVector& vIndices, const VertexVector& vVertices);
        virtual ~Mesh2();

        // Getter and Setters
        const std::string& getName() const;
        void getIndices(IndexVector& vIndices) const;
        void getPositions(std::vector<glm::vec3>& vPositions) const;
        void getNormals(std::vector<glm::vec3>& vNormals) const;
        void getTexCoords(std::vector<glm::vec2>& vTexCoords) const;

    // Member variables
    private:
        std::string  name_;
        IndexVector  vIndices_;
        VertexVector vVertices_;;
};

#endif /* MESH2_HPP_ */
