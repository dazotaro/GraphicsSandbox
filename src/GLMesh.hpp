/*
 * GLMesh.hpp
 *
 *  Created on: May 7, 2013
 *      Author: jusabiaga
 */

#ifndef GLMESH_HPP_
#define GLMESH_HPP_

#include "GL/glew.h"    // GL and GLEW
#include "Mesh.hpp"     // Mesh

/**
 * @brief      Mesh adapter to OpenGL.
 *
 * @details    This should be a 'Flyweight' pattern, so models only exist once. It needs to:
 *              + Load the Mesh into GL's VBOs (Vertex Buffer Object) and VAO (Vertex Array Object)
 *              + It needs to draw the Mesh in OpenGL
 *              There should only be a GLMesh per Mesh object; if we want to have two instances of the same model,
 *              this is accomplish by creating to GLMeshInstance objects, both sharing the same GLMesh under the hood.
 *
 * \todo Turn into 'flyweight'
 */
class GLMesh : public Mesh
{
    public:
        explicit GLMesh(const Mesh &mesh);
        GLMesh(const std::string  &name,
               const PositionList &positions,
               const NormalList   &normals,
               const TangentList  &tangents,
               const ColorList    &colors,
               const TexCoordList &tex_coords,
               const FaceList     &faces);
        virtual ~GLMesh();

        virtual void draw(void) const;
        bool init(void);
        bool initVBOs(void);

    private:
        GLuint vao_handle_;         //!< Handle to VAO
        GLuint *vbo_handles_;
};

#endif /* GLMESH_HPP_ */
