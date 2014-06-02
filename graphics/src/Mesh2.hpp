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
#include <JU/Defs.hpp>		// uint32



class Mesh2
{
    // TYPEDEFS and Structure Definitions
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
