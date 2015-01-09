/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "GLSceneParticles.hpp"     // GLSceneParticles
#include "CameraInterface.hpp"      // camera_Interface
#include "CameraThirdPerson.hpp"    // CameraThirdPerson
#include "GLParticleSystem.hpp"		// GLParticleSystem

// Global includes
#include <JU/core/Object3D.hpp>     // Object3D
#include <JU/core/Defs.hpp>			// uint32

GLSceneParticles::GLSceneParticles(int width, int height) : GLScene(width, height),
                                     camera_gps_(0), camera_target_(0), camera_(0),
                                     camera_controller_(width, height, 0.5f),
                                     gl_particle_system_(0)
{
}



GLSceneParticles::~GLSceneParticles()
{
	delete gl_particle_system_;
}



/**
* @brief Initialized the Scene
*/
void GLSceneParticles::init(void)
{
    // GLSL PROGRAMS
    // -------------
    glsl_program_map_["simple"]  = compileAndLinkShader("shaders/simpleMVP.vert", "shaders/simple.frag");

    current_program_iter_ = glsl_program_map_.find("simple");
    
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)

    // CAMERA
    // ------
    glm::vec3 camera_position (0.0f, 20.0f, 10.0f);
    glm::vec3 camera_z = glm::normalize(camera_position);
    glm::vec3 camera_x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_z));
    glm::vec3 camera_y = glm::normalize(glm::cross(camera_z, camera_x));
    camera_gps_ = new Object3D(camera_position, // camera_'s position (eye's coordinates)
                               camera_x, // camera_'s X axis
                               camera_y, // camera_'s Y axis
                               camera_z);// VIEWING AXIS (the camera_ is looking into its NEGATIVE Z axis)
    camera_target_ = new Object3D(glm::vec3(0.f, 0.f, 0.f), // target's position (eye's coordinates)
                                  glm::vec3(1.f, 0.f, 0.f), // target's X axis
                                  glm::vec3(0.f, 1.f, 0.f), // target's X axis
                                  glm::vec3(0.f, 0.f, 1.f)); // target's X axis
    tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 0.5f, 1000.f), *camera_target_);
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);

    // PARTICLES: Initial positions
    // ----------------------------
    gl_particle_system_ = new GLParticleSystem();

    JU::uint32 num_particles = NUM_ROWS * NUM_COLS;
    gl_particle_system_->init(num_particles);

    JU::f32 delta_x = 2.0f  / NUM_COLS;
    JU::f32 delta_y = 2.0f / NUM_ROWS;

    positions_.resize(num_particles);
    for (JU::uint16 i = 0; i < NUM_ROWS; i++)
    {
        for (JU::uint16 j = 0; j < NUM_COLS; j++)
        {
            positions_[i * NUM_COLS + j] = glm::vec3(-1.0f + j * delta_x, -1.0f + i * delta_y, 0.f);
        }
    }

    colors_.resize(num_particles, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    gl_particle_system_->setParticleData(positions_, colors_);

}



/**
* @brief Update everything that needs to be updated in the scene
*
* @param time Time elapsed since the last update
*
* @todo Investigate using glMapBufferRange instead of glBufferSubData to improve performance
*/
void GLSceneParticles::update(float time)
{
	static JU::f32 speed = 0.0001f;
	static Object3D look_at(glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(1.0f, 0.0f, 0.0f),
							glm::vec3(0.0f, 1.0f, 0.0f),
							glm::vec3(0.0f, 0.0f, 1.0f));

    float radius_delta, angle;
    glm::vec3 axis;
    camera_controller_.update(radius_delta, angle, axis);

    // Convert the axis from the camera to the world coordinate system
    axis = glm::vec3(tp_camera_->getTransformToParent() * glm::vec4(axis, 0.0f));
    tp_camera_->update(look_at, radius_delta, angle, axis);

	// PARTICLES:
	if (positions_[0].x > 0.0f)
		speed = -speed;

    for (JU::uint16 i = 0; i < NUM_PARTICLES; i++)
    {
            positions_[i].x += time * speed;
    }

    gl_particle_system_->set_positions(positions_);
}



/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneParticles::render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    current_program_iter_->second.use();

    // Model Matrix
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(tp_camera_->getViewMatrix());
    // Perspective Matrix
    glm::mat4 P(tp_camera_->getPerspectiveMatrix());

    // Draw the whole particle system
    gl_particle_system_->draw(current_program_iter_->second, M, V, P);
}



/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneParticles::resize(int width, int height)
{
    GLScene::resize(width, height);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    tp_camera_->setAspectRatio(static_cast<float>(width)/height);
    camera_controller_.windowResize(width, height);
}



/**
* @brief Handle keyboard input
*
* @param key    Id of key pressed
* @param x      Location of the mouse when the key was pressed
* @param y      Location of the mouse when the key was pressed
*/
void GLSceneParticles::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '0':
            if(++current_program_iter_ == glsl_program_map_.end())
                current_program_iter_ = glsl_program_map_.begin();
            break;

        default:
            break;
    }
}



void GLSceneParticles::mouseClick(int button, int state, int x, int y)
{
	camera_controller_.mouseClick(button, state, x, y);
}



void GLSceneParticles::mouseMotion(int x, int y)
{
	camera_controller_.mouseMotion(x, y);
}



void GLSceneParticles::clear(void)
{
    delete camera_gps_;
    delete camera_target_;
    //delete fp_camera_;
    delete tp_camera_;
}


