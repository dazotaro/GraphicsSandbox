/*
 * GLSceneCometTail.cpp
 *
 *      Author: jusabiaga
 */

// Local includes
#include "GLSceneCometTail.hpp"         // GLSceneCometTail
#include <JU/graphics/Object3D.hpp>              // Object3D
#include <JU/graphics/CameraInterface.hpp>       // camera_Interface
#include <JU/graphics/CameraThirdPerson.hpp>     // CameraThirdPerson

// Global includes
#include <GL/freeglut.h>                // GLUT_LEFT_BUTTON

GLSceneCometTail::GLSceneCometTail(JU::int16 width, JU::int16 height, JU::uint16 max_tail_count)
                                   : GLScene(width, height),
                                     max_tail_count_(max_tail_count), is_tail_active_(false), tail_index_(max_tail_count),
                                     camera_gps_(0), camera_target_(0), camera_(0),
                                     camera_controller_(width, height, M_PI/4.0f, M_PI/4.0f, 0.2f)
{
    vecposition_.reserve(max_tail_count_);
    veccolor_.reserve(max_tail_count_);
}



GLSceneCometTail::~GLSceneCometTail()
{
    // TODO Auto-generated destructor stub
}



/**
* @brief Initialized the Scene
*/
void GLSceneCometTail::init(void)
{
    // GLSL PROGRAMS
    // -------------
    glsl_program_map_["simple"]  = compileAndLinkShader("shaders/simple.vert", "shaders/simple.frag");
    //glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");
    //glsl_program_map_["perfragment_halfway"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag_halfway.fs");
    //glsl_program_map_["perfragment_texture"] = compileAndLinkShader("shaders/perfrag_texture.vs", "shaders/perfrag_texture.fs");

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
    tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), *camera_target_);
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);


    // Initialize the gl particle system drawable object
    gl_particle_system_.init(max_tail_count_);
}



/**
* @brief Update everything that needs to be updated in the scene
*
* @param time Time elapsed since the last update
*
* @todo Investigate using glMapBufferRange instead of glBufferSubData to improve performance
*/
void GLSceneCometTail::update(float time)
{
    /*
	static JU::f32 speed = 0.0001f;

	float radius_delta, inclination_delta, azimuth_delta;
	camera_controller_.update(radius_delta, inclination_delta, azimuth_delta);

	tp_camera_->update(*camera_target_, radius_delta, inclination_delta, azimuth_delta);
    */

    gl_particle_system_.setParticleData(vecposition_, veccolor_);
}



/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneCometTail::render(void)
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

    // Draw each object
    gl_particle_system_.draw(current_program_iter_->second, M, V, P);
}



/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneCometTail::resize(int width, int height)
{
    GLScene::resize(width, height);
    camera_->setAspectRatio(static_cast<float>(width)/height);
}



/**
* @brief Handle keyboard input
*
* @param key    Id of key pressed
* @param x      Location of the mouse when the key was pressed
* @param y      Location of the mouse when the key was pressed
*/
void GLSceneCometTail::keyboard(unsigned char key, int x, int y)
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



void GLSceneCometTail::mouseClick(int button, int state, int x, int y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                is_tail_active_ = true;
            }
            else if (state == GLUT_UP)
            {
                is_tail_active_ = false;
            }
            break;

        default:
            //camera_controller_.mouseClick(button, state, x, y);
            break;
    }
}



void GLSceneCometTail::mouseMotion(int x, int y)
{
    if (is_tail_active_)
    {
        static JU::f32 inv_hwidth  = 2.0 / width_;
        static JU::f32 inv_hheight = 2.0 / height_;

        glm::vec3 position(x * inv_hwidth - 1.0f, -y * inv_hheight + 1.0f, 0.0f);
        glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);

        // if the vector is not yet full: INSERT
        if (vecposition_.size() < max_tail_count_)
        {
            vecposition_.push_back(position);
            veccolor_.push_back(color);
        }
        else // REPLACE
        {
            // Go back to the beginning
            if (tail_index_ >= max_tail_count_)
                tail_index_ = 0;

            vecposition_[tail_index_] = position;
            veccolor_[tail_index_]    = color;
        }

        ++tail_index_;
    }

    //camera_controller_.mouseMotion(x, y);
}



void GLSceneCometTail::clear(void)
{
    delete camera_gps_;
    delete camera_target_;
    //delete fp_camera_;
    delete tp_camera_;
}

