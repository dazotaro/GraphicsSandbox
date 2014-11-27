/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENEPARTICLES_HPP_
#define GLSCENEPARTICLES_HPP_

// Local includes
#include "GLScene.hpp"      	    // GLScene
#include "ArcBallController.hpp" // ArcBallController

// Global includes
#include <JU/core/Defs.hpp>              // uint16
#include <vector>                   // std::vector

// Forward Declarations
class GLMesh;
class GLMeshInstance;
class Node3D;
class Object3D;
class CameraInterface;
class CameraFirstPerson;
class CameraThirdPerson;
class GLParticleSystem;

/*
 * @brief Scene class
 *
 * \todo Maybe unnecessary class
 */
class GLSceneParticles : public GLScene
{
    public:
        static const JU::int16 POSITION_VECTOR_SIZE     = 3;
        static const JU::int16 COLOR_VECTOR_SIZE        = 4;
        static const JU::uint16 NUM_ROWS             = 10;
        static const JU::uint16 NUM_COLS             = 10;
        static const JU::uint16 NUM_PARTICLES        = NUM_ROWS * NUM_COLS;

    public:
        GLSceneParticles(int width, int height);
        virtual ~GLSceneParticles();

        void init(void);
        void update(float time);
        void render(void);
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        void resize(int width, int height);

    private:
        void cleanup(void);

    private:
        Object3D*        camera_gps_;
        Object3D*        camera_target_;
        CameraThirdPerson *tp_camera_;
        CameraInterface* camera_;
        ArcBallController camera_controller_;

        GLParticleSystem* gl_particle_system_;
        std::vector<glm::vec3> positions_;
        std::vector<glm::vec4> colors_;
};

#endif /* GLSCENEPARTICLES_HPP_ */
