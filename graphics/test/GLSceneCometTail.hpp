/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENECOMETTAIL_HPP_
#define GLSCENECOMETTAIL_HPP_

// Local includes
#include <JU/GLScene.hpp>      	    // GLScene
#include <JU/ArcBallController.hpp> // ArcBallController
#include <JU/GLParticleSystem.hpp>  // GLParticleSystem

// Global includes
#include <JU/Defs.hpp>              // uint16
#include <vector>                   // std::vector

// Forward Declarations
class Object3D;
class CameraInterface;
class CameraThirdPerson;

/*
 * @brief Scene class to demo particle system of tail to the mouse pointer
 *
 * \todo Maybe unnecessary class
 */
class GLSceneCometTail : public GLScene
{
    public:
        GLSceneCometTail(JU::int16 width, JU::int16 height, JU::uint16 max_tail_count);
        virtual ~GLSceneCometTail();

        // GLScene
        //--------
        void init(void);
        void update(float time);
        void render(void) const;
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        // override
        void resize(int width, int height);

    private:
        void cleanup(void);

    private:
        // Comet tail particle system variables
        JU::uint16          max_tail_count_;        // max number of particles
        bool                is_tail_active_;        // is the comet tail active
        JU::uint16          tail_index_;            // index to the last particle in the comet tail
        GLParticleSystem    gl_particle_system_;    // gl object to draw all particles

        Object3D*           camera_gps_;
        Object3D*           camera_target_;
        CameraThirdPerson*  tp_camera_;
        CameraInterface*    camera_;
        ArcBallController   camera_controller_;

        std::vector<glm::vec3> vecposition_;
        std::vector<glm::vec4> veccolor_;
};

#endif /* GLSCENECOMETTAIL_HPP_ */
