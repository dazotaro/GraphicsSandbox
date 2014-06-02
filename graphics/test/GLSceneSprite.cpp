/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#include "GLSceneSprite.hpp"      // GLSceneLighting
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "Object3D.hpp"             // Object3D
#include "CameraInterface.hpp"      // CameraInterface
#include "CameraFirstPerson.hpp"    // CameraFirstPerson
#include "ShapeHelper.hpp"          // build Mesh helper funtions
#include "TextureManager.hpp"       // loadTexture()
#include <glm/gtc/matrix_transform.hpp>

GLSceneSprite::GLSceneSprite(int width, int height) : GLScene(width, height),
										gl_plane_(0),
										gl_plane_instance_(0),
										root_node_(0),
										camera_gps_(0),
										camera_(0)
{
    child_node_ = 0;
}

GLSceneSprite::~GLSceneSprite()
{
    // TODO Auto-generated destructor stub
}

/**
* @brief Initialized the Scene
*/
void GLSceneSprite::init(void)
{
    glsl_program_map_["sprite"]  = compileAndLinkShader("shaders/sprite.vert", "shaders/sprite.frag");

    current_program_iter_ = glsl_program_map_.find("sprite");
    
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)

    // Create Cube Mesh
    gl_plane_ = new GLMesh(Graphics::buildMesh(Graphics::PLANE));
    // Load the Mesh into VBO and VAO
    gl_plane_->init();
    // Create instance of GLMEsh (there could be more than one)
    gl_plane_instance_ = new GLMeshInstance(gl_plane_, 1.0f, 1.0f, 1.0f);

    TextureManager::loadTexture("test", "texture/test.tga");
    gl_plane_instance_->addColorTexture("test");

    // Give the cube a position and orientation
    Object3D root_cube(glm::vec3(0.0f, 0.0f, -5.0f), // Model's position
                       glm::vec3(1.0f, 0.0f,  0.0f), // Model's X axis
                       glm::vec3(0.0f, 1.0f,  0.0f), // Model's Y axis
                       glm::vec3(0.0f, 0.0f,  1.0f));// Model's Z axis

    // No children
    NodePointerList children;
    // Create Node
    root_node_ = new Node3D(root_cube, gl_plane_instance_, children, true);

    // Create the camera_
    camera_gps_ = new Object3D(glm::vec3(0.0f, 0.0f, 0.0f), // camera_'s position (eye's coordinates)
                               glm::vec3(1.0f, 0.0f, 0.0f), // camera_'s X axis
                               glm::vec3(0.0f, 1.0f, 0.0f), // camera_'s Y axis
                               glm::vec3(0.0f, 0.0f, 1.0f));// VIEWING AXIS (the camera_ is looking into its NEGATIVE Z axis)
    CameraFirstPerson *fp_camera_ = new CameraFirstPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), *camera_gps_);
    camera_ = dynamic_cast<CameraInterface *>(fp_camera_);
}

/**
* @brief Update everything that needs to be updated in the scene
*
* @param time Time elapsed since the last update
*/
void GLSceneSprite::update(float time)
{
    // camera_: update position and orientation
    //camera_->update(*camera_gps_);
    // LIGHTS: update position

}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneSprite::render(void)
{
    glClearColor(0.0f, 0.1f, 0.1f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    current_program_iter_->second.use();
    
    // Model Matrix
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(camera_->getViewMatrix());
    // Perspective Matrix
    glm::mat4 P(glm::ortho(-4.5f, 4.5f, -3.0f, 3.0f));    // Draw each object
    root_node_->draw(current_program_iter_->second, M, V, P);
}

/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneSprite::resize(int width, int height)
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
void GLSceneSprite::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '0':
            if(++current_program_iter_ == glsl_program_map_.end())
                current_program_iter_ = glsl_program_map_.begin();
            break;

        case 'a':
        case 'A':
            root_node_->rotateX(-1.0f);
            break;

        case 'd':
        case 'D':
            root_node_->rotateX(1.0f);
            break;

        case 'q':
        case 'Q':
            root_node_->rotateY(1.0f);
            break;

        case 'e':
        case 'E':
            root_node_->rotateY(-1.0f);
            break;

        case 'w':
        case 'W':
            root_node_->rotateZ(-1.0f);
            break;

        case 's':
        case 'S':
            root_node_->rotateZ(1.0f);
            break;

        case 'k':
        case 'K':
            camera_gps_->rotateY(1.0f);  // Yaw
            break;

        case ';':
        case ':':
            camera_gps_->rotateY(-1.0f);  // Yaw
            break;

        case 'i':
        case 'I':
            camera_gps_->rotateZ(1.0f);  // Roll
            break;

        case 'p':
        case 'P':
            camera_gps_->rotateZ(-1.0f); // Roll
            break;

        case 'o':
        case 'O':
            camera_gps_->rotateX(-1.0f);  // Pitch
            break;

        case 'l':
        case 'L':
            camera_gps_->rotateX(1.0f); // Pitch
            break;

        case 'u':
        case 'U':
            camera_gps_->translate(glm::vec3(0.0f, 0.0f, -0.1f));
            break;

        case 'j':
        case 'J':
            camera_gps_->translate(glm::vec3(0.0f, 0.0f, 0.1f));
            break;
    }
}

void GLSceneSprite::mouseClick(int button, int state, int x, int y)
{

}

void GLSceneSprite::cleanup(void)
{
    delete gl_plane_instance_;
    delete gl_plane_;
    delete root_node_;
    delete child_node_;
    delete camera_gps_;
    delete camera_;
}


