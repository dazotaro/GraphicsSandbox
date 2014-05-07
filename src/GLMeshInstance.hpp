/*
 * GLMeshInstance.hpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#ifndef GLMESHINSTANCE_HPP_
#define GLMESHINSTANCE_HPP_

#include "gl_core_4_2.h"                // glLoadGen generated header file
#include <string>               // std:string
#include <map>                  // std::map
#include "DrawInterface.hpp"    // DrawInterface

// Forward Declarations
class GLSLProgram;
class GLMesh;

/**
 * @brief It contains a given instance of a GLMesh.
 *
 * @details There is a single GLMesh per Mesh object, but there can be several GLMeshInstance objects that share a GLMesh
 *          (e.g. a cube Mesh) but have different scale factors. The GLMeshInstance contains the handle to the Shader Program because
 *          two GLMeshInstance that shared the same GLMesh (e.g. two cubes, with or without the same scale factors, might require two
 *          different Shader Programs (e.g. to apply different lighting effects).
 */
class GLMeshInstance : public DrawInterface
{
    public:
        explicit GLMeshInstance(const GLMesh *mesh,
                                float scaleX = 1.0f,
                                float scaleY = 1.0f,
                                float scaleZ = 1.0f);
        ~GLMeshInstance();

        void addColorTexture(const std::string &texture_name);
        void addNormalTexture(const std::string &texture_name);

        void draw(const GLSLProgram &program, const glm::mat4 & model, const glm::mat4 &view, const glm::mat4 &projection) const;

    private:
        const GLMesh *mesh_;                //!< Shared Mesh object
        float scaleX_;                      //!< Scale factor in the X axis
        float scaleY_;                      //!< Scale factor in the Y axis
        float scaleZ_;                      //!< Scale factor in the Z axis
        std::vector<std::string> color_texture_name_list_;
        std::string normal_map_texture_name_;

};

#endif /* GLMESHINSTANCE_HPP_ */
