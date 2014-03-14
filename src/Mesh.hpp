/*
 * Mesh.h
 *
 *  Created on: May 2, 2013
 *      Author: jusabiaga
 */

#ifndef MESH_H_
#define MESH_H_

#include <string>      // std::string
#include <vector>
#include <glm/glm.hpp>  // glm vector and matrix types


/**
 * @brief      Contains all information of a Mesh.
 *
 * @details    Contains faces which in turn are made out of vertex positions, colors, texture coordinates and normals.
 */
class Mesh
{
    public:

        // CONSTANT DEFINITIONS
        static const unsigned int POSITION_VECTOR_SIZE     = 3;
        static const unsigned int NORMAL_VECTOR_SIZE       = 3;
        static const unsigned int TANGENT_VECTOR_SIZE      = 4;
        static const unsigned int COLOR_VECTOR_SIZE        = 4;
        static const unsigned int TEX_COORDS_VECTOR_SIZE   = 2;

        // TYPEDEFS
        typedef std::vector<unsigned int> IndexList;
        typedef IndexList::const_iterator IndexListConstIterator;

        /**
         * @brief      Contains the information to define a Face
         *
         * @details    Contains the indices to the lists of positions, normals...
         */
        class Face
        {
            public:

                enum IndexType
                {
                    VERTEX_INDEX,
                    COLOR_INDEX,
                    TEXTURE_INDEX
                };

                Face () {}
                Face (IndexList vertex_indices,
                      IndexList color_indices,
                      IndexList tex_coord_indices)
                {
                    setIndices(VERTEX_INDEX,  vertex_indices);
                    setIndices(COLOR_INDEX,   color_indices);
                    setIndices(TEXTURE_INDEX, tex_coord_indices);
                }

                void setIndices(IndexType index_type, const IndexList &index_list)
                {
                    switch(index_type)
                    {
                    case VERTEX_INDEX:
                        vertex_indices_ = index_list;
                        break;

                    case COLOR_INDEX:
                        color_indices_ = index_list;
                        break;

                    case TEXTURE_INDEX:
                        tex_coord_indices_ = index_list;
                        break;

                    default:
                        break;

                    }
                }

                void getIndices(IndexType index_type, IndexList &index_list) const
                {
                    switch(index_type)
                    {
                    case VERTEX_INDEX:
                        index_list = vertex_indices_;
                        break;

                    case COLOR_INDEX:
                        index_list = color_indices_;
                        break;

                    case TEXTURE_INDEX:
                        index_list = tex_coord_indices_;
                        break;

                    default:
                        break;

                    }
                }

            private:
                // Member variables
                IndexList vertex_indices_;      //!< Indices to the list of vertex/normal/tangent coordinates
                IndexList color_indices_;       //!< Indices to the list of vertex colors
                IndexList tex_coord_indices_;   //!< Indices to the list of vertex texture coordinates
        };

        /**
         * @brief      Wrapper over the Face class to provide a simplified constructor for Triangles.
         */
        class TriangleFace : public Face
        {
            public:
                TriangleFace (unsigned int vertex0, unsigned int vertex1, unsigned vertex2,
                              unsigned int color0,  unsigned int color1,  unsigned color2,
                              unsigned int tex0,    unsigned int tex1,    unsigned tex2)
                {
                    std::vector<unsigned int> index_list(3);

                    index_list[0] = vertex0;
                    index_list[1] = vertex1;
                    index_list[2] = vertex2;
                    setIndices(VERTEX_INDEX, index_list);

                    index_list[0] = color0;
                    index_list[1] = color1;
                    index_list[2] = color2;
                    setIndices(COLOR_INDEX, index_list);

                    index_list[0] = tex0;
                    index_list[1] = tex1;
                    index_list[2] = tex2;
                    setIndices(TEXTURE_INDEX, index_list);
                }
        };

        // TYPEDEFS
        typedef std::vector<glm::vec3>          PositionList;
        typedef PositionList::const_iterator    PositionListConstIterator;
        typedef std::vector<glm::vec3>          NormalList;
        typedef NormalList::const_iterator      NormalListConstIterator;
        typedef NormalList::iterator            NormalListIterator;
        typedef std::vector<glm::vec4>          TangentList;
        typedef TangentList::const_iterator     TangentListConstIterator;
        typedef TangentList::iterator           TangentListIterator;
        typedef std::vector<glm::vec4>          ColorList;
        typedef ColorList::const_iterator       ColorListConstIterator;
        typedef std::vector<glm::vec2>          TexCoordList;
        typedef TexCoordList::const_iterator    TexCoordListConstIterator;
        typedef std::vector<Face>               FaceList;
        typedef FaceList::const_iterator        FaceListConstIterator;

        Mesh(const std::string  &name,
             const PositionList &positions,
             const NormalList   &normals,
             const TangentList  &tangents,
             const ColorList    &colors,
             const TexCoordList &tex_coords,
             const FaceList     &faces);

        virtual ~Mesh();

        // GETTERS
        const ColorList& getColors() const;
        const FaceList& getFaces() const;
        const std::string& getName() const;
        const NormalList& getNormals() const;
        const TangentList& getTangents() const;
        const PositionList& getPositions() const;
        const TexCoordList& getTexCoords() const;

        // EXPORT AND OUTPUT FUNCTIONS
        void exportOBJ(void) const;
        void export2OBJ(const char *filename) const;
        friend std::ostream& operator<<(std::ostream &out, const Mesh &rhs);

    private:

        std::string     name_;          //!< ID of the Mesh
        PositionList    positions_;     //!< List of vertex coordinates
        NormalList      normals_;       //!< List of vertex normals
        TangentList     tangents_;      //!< List of vertex tangent vectors
        ColorList       colors_;        //!< List of vertex colors
        TexCoordList    tex_coords_;    //!< List of vertex texture coordinates
        FaceList        faces_;         //!< List of faces
};

#endif /* MESH_H_ */
