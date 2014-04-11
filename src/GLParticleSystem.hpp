/*
 * GLParticleSystem.hpp
 *
 *  Created on: Apr 7, 2014
 *      Author: jusabiaga
 */

#ifndef GLPARTICLESYSTEM_HPP_
#define GLPARTICLESYSTEM_HPP_

// Local includes
#include "DrawInterface.hpp"    // DrawInterface parent class

// Global includes
#include <JU/Defs.hpp>          // uint32
#include <GL/glew.h>            // GL and GLEW
#include <glm/glm.hpp>          // glm::vec3/vec4
#include <vector>               // std::vector

class GLParticleSystem: public DrawInterface
{
    public:
        static const GLint POSITION_VECTOR_SIZE = 3;
        static const GLint COLOR_VECTOR_SIZE    = 4;

    public:
        GLParticleSystem();
        virtual ~GLParticleSystem();

        bool init(JU::uint32 num_particles);

        void setParticleData(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors);
        void set_positions(const std::vector<glm::vec3>& positions);
        void set_colors(const std::vector<glm::vec4>& colors);

        // DrawInterface
        // -------------
        virtual void draw(const GLSLProgram &program,
                          const glm::mat4 & model,
                          const glm::mat4 &view,
                          const glm::mat4 &projection) const;

    private:
        bool initVBOs(void);

    private:
        JU::uint32 max_particles_;          //!< Max number of particles supported
        // Buffer handles
        GLuint vao_handle_;                 //!< Handle to VAO
        GLuint *vbo_handles_;               //!< Handles to VBOs
        // Particle data
        std::vector<glm::vec3> positions_;  //!< Vector of positions
        std::vector<glm::vec4> colors_;     //!< Vector of colors
};

#endif /* GLPARTICLESYSTEM_HPP_ */
