/*
 * GLMeshInstance.hpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#ifndef GLMESHINSTANCE_HPP_
#define GLMESHINSTANCE_HPP_

// Local Includes
#include "gl_core_4_2.h"                // glLoadGen generated header file
#include "DrawInterface.hpp"    // DrawInterface

// Global Includes
#include "common/Defs.hpp"			// JU::f32
#include <string>               // std:string
#include <map>                  // std::map

// Forward Declarations
class GLSLProgram;
class GLMesh;
class Material;

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
		GLMeshInstance() : mesh_(0), scaleX_(1.0f), scaleY_(1.0f), scaleZ_(1.0f), material_(0) {}

        GLMeshInstance(const GLMesh* mesh,
                       JU::f32 scaleX = 1.0f,
                       JU::f32 scaleY = 1.0f,
                       JU::f32 scaleZ = 1.0f,
                       const Material* material = 0);
        ~GLMeshInstance();

        void addColorTexture(const std::string &texture_name);
        void addNormalTexture(const std::string &texture_name);
        void setMesh(const GLMesh* mesh);
        void setScale(JU::f32 x, JU::f32 y, JU::f32 z);
        void setMaterial(const Material* material);

        void draw(const GLSLProgram &program,
        		  const glm::mat4 & model,
        		  const glm::mat4 &view,
        		  const glm::mat4 &projection) const;

    private:
        const GLMesh* mesh_;                //!< Shared Mesh object
        JU::f32 scaleX_;                      //!< Scale factor in the X axis
        JU::f32 scaleY_;                      //!< Scale factor in the Y axis
        JU::f32 scaleZ_;                      //!< Scale factor in the Z axis
        Material* material_;					//!< Material coefficients
        std::vector<std::string> color_texture_name_list_;
        std::string normal_map_texture_name_;

};

#endif /* GLMESHINSTANCE_HPP_ */
