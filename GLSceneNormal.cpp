/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#include "GLSceneNormal.hpp"      // GLSceneNormal
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "Object3D.hpp"             // Object3D
#include "CameraInterface.hpp"      // CameraInterface
#include "CameraFirstPerson.hpp"    // CameraFirstPerson
#include "CameraThirdPerson.hpp"    // CameraFirstPerson
#include "ShapeHelper.hpp"          // build Mesh helper funtions
#include <SOIL.h>                   // SOIL_load_image
#include "NormalMapHelper.hpp"      // convertHeightMapToNormalMap()
#include <glm/gtx/transform2.hpp>   // glm::rotate, glm::translate
#include "TextureManager.hpp"       // TextureManager static class
#include "DebugGlm.hpp"				// operator<<

GLSceneNormal::GLSceneNormal(int width, int height) : GLScene(width, height),
								 gl_cube_(0),
                                 gl_cube_instance_(0),
                                 camera_gps_(0),
                                 tp_camera_(0),
                                 camera_(0),
                                 gl_sphere_(0),
                                 gl_sphere_instance_(0)
{
}

GLSceneNormal::~GLSceneNormal()
{
    // TODO Auto-generated destructor stub
}

/**
* @brief Initialized the Scene
*/
void GLSceneNormal::init(void)
{
    // OPENGL SETUP
    // --------------
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)

	// SHADERS: load, compile and link them
    // --------------
	glsl_program_map_["perfragment_texture"] = compileAndLinkShader("shaders/perfrag_texture.vs", "shaders/perfrag_texture.fs");
    glsl_program_map_["normal_mapping"] = compileAndLinkShader("shaders/normalmap.vs", "shaders/normalmap.fs");

    current_program_iter_ = glsl_program_map_.find("normal_mapping");

    // TEXTURE LOADING
    // --------------
    TextureManager::loadTexture("normal_map", "texture/multi_normal_map.tga");
    TextureManager::loadTexture("light", "texture/light_texture.tga");
    TextureManager::loadTexture("test", "texture/test.tga");

    // OBJECT LOADING
    // --------------
    // 1- CUBE
    // Create Cube Mesh
    gl_cube_ = new GLMesh(Graphics::buildMesh(Graphics::SPHERE, 100, 100));
    // and LOAD the Mesh into VBO and VAO
    gl_cube_->init();
    // Create instance of GLMesh (there could be more than one)
    gl_cube_instance_ = new GLMeshInstance(gl_cube_, 10.0f, 15.0f, 10.0f);

    gl_cube_instance_->addColorTexture("test");
    gl_cube_instance_->addNormalTexture("normal_map");

    // Give the cube a position and orientation
    Object3D root_cube(glm::vec3(0.0f, 0.0f, 0.0f), // Model's position respect to the parent
                       glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis
                       glm::vec3(0.0f, 1.0f, 0.0f), // Model's Y axis
                       glm::vec3(0.0f, 0.0f, 1.0f));// Model's Z axis
    NodePointerList children;
    Node3D *cubes = new Node3D(root_cube, gl_cube_instance_, children, true);

    node_map_["cubes"] = cubes;

    // Create the Camera
    camera_gps_ = new Object3D(glm::vec3(0.0f, 0.0f, 5.0f), // Camera's position (eye's coordinates)
                               glm::vec3(1.0f, 0.0f, 0.0f), // Camera's X axis
                               glm::vec3(0.0f, 1.0f, 0.0f), // Camera's Y axis
                               glm::vec3(0.0f, 0.0f, 1.0f));// VIEWING AXIS (the camera is looking into its NEGATIVE Z axis)
    CameraFirstPerson *fp_camera = new CameraFirstPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), *camera_gps_);
    camera_ = dynamic_cast<CameraInterface *>(fp_camera);
    tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f),
                                                         dynamic_cast<Object3D &>(*cubes),
                                                         10.0f, 0.0f);

    /*
    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * vec4(5.0f,5.0f,2.0f,1.0f) );
    */

    // LIGHTS
    //---------
    glm::vec3 light_position (0.0f, 0.0f, 10.0f);
    glm::vec3 light_intensity (1.0f, 1.0f, 1.0f);
    // Create Cube Mesh
    gl_sphere_ = new GLMesh(Graphics::buildMesh(Graphics::SPHERE));
    // Load the Mesh into VBO and VAO
    gl_sphere_->init();
    // Create instance of GLMEsh (there could be more than one)
    gl_sphere_instance_ = new GLMeshInstance(gl_sphere_, 1.0f, 1.0f, 1.0f);
    // Color texture for light object
    gl_sphere_instance_->addColorTexture("light");

    Object3D root_sphere(light_position,
                         glm::vec3(1.0f, 0.0f,  0.0f), // Model's X axis
                         glm::vec3(0.0f, 1.0f,  0.0f), // Model's Y axis
                         glm::vec3(0.0f, 0.0f,  1.0f));// Model's Z axis
    NodePointerList light_children;
    Node3D *light_node = new Node3D(root_sphere, gl_sphere_instance_, light_children, true);

    node_map_["light"] = light_node;

    lights_positional_.push_back(LightPositional(light_position, light_intensity));
}

void GLSceneNormal::loadMaterial(void) const
{
    (current_program_iter_->second).setUniform("Kd", 0.4f, 0.4f, 0.4f);
    (current_program_iter_->second).setUniform("Ks", 0.9f, 0.9f, 0.9f);
    (current_program_iter_->second).setUniform("Ka", 0.2f, 0.2f, 0.2f);
    (current_program_iter_->second).setUniform("Shininess", 10.0f);
}

void GLSceneNormal::loadLights(void) const
{
    // WARNING: The shader expects the light position in eye coordinates
	(current_program_iter_->second).setUniform("Light.Position",  tp_camera_->getViewMatrix() * glm::vec4(lights_positional_[0].position_,1.0f));
    (current_program_iter_->second).setUniform("Light.Intensity", lights_positional_[0].intensity_);

    /*
    // 0
    (current_program_iter_->second).setUniform("lights[0].Position",  glm::vec4(5.0f,0.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[0].Intensity", glm::vec3(0.8f,0.8f,0.8f));
    // 1
    (current_program_iter_->second).setUniform("lights[1].Position",  glm::vec4(0.0f,5.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[1].Intensity", glm::vec3(0.8f,0.8f,0.8f));
    // 2
    (current_program_iter_->second).setUniform("lights[2].Position",  glm::vec4(-5.0f,0.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[2].Intensity", glm::vec3(0.8f,0.8f,0.8f));
    // 3
    (current_program_iter_->second).setUniform("lights[3].Position",  glm::vec4(0.0f,-5.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[3].Intensity", glm::vec3(0.0f,0.0f,0.0f));
    // 4
    (current_program_iter_->second).setUniform("lights[4].Position",  glm::vec4(0.0f,0.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[4].Intensity", glm::vec3(0.0f,0.0f,0.0f));
    // 5
    (current_program_iter_->second).setUniform("lights[5].Position",  glm::vec4(0.0f,0.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[5].Intensity", glm::vec3(0.0f,0.0f,0.0f));
    // 6
    (current_program_iter_->second).setUniform("lights[6].Position",  glm::vec4(0.0f,0.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[6].Intensity", glm::vec3(0.0f,0.0f,0.0f));
    // 7
    (current_program_iter_->second).setUniform("lights[7].Position",  glm::vec4(0.0f,0.0f,0.0f,1.0f));
    (current_program_iter_->second).setUniform("lights[7].Intensity", glm::vec3(0.0f,0.0f,0.0f));
    */
    //}
}

/**
* @brief Update everything that needs to be updated in the scene
*
* @param time Time elapsed since the last update (in milliseconds)
*/
void GLSceneNormal::update(float time)
{
    // CAMERA: update position and orientation
    ///camera_->update(*camera_gps_);
    tp_camera_->update(dynamic_cast<Object3D &>(*(node_map_["cubes"])));
    // LIGHTS: update position
    static const float angle_speed = (360 * 0.1f) * 0.001f ; // 20 seconds to complete a revolution

    glm::mat4 rotation = glm::rotate(glm::mat4(1.f), angle_speed * time, glm::vec3(0.0f, 1.0f, 0.0f));
    for (LightPositionalIterator light = lights_positional_.begin(); light != lights_positional_.end(); ++light)
    {
        glm::vec4 position = rotation * glm::vec4(light->position_, 0.0f);
        light->position_.x = position.x;
        light->position_.y = position.y;
        light->position_.z = position.z;

        node_map_["light"]->setPosition(light->position_);
    }
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneNormal::render(void) const
{
    glClearColor(0.0f, 0.1f, 0.1f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    current_program_iter_->second.use();

    // LOAD MATERIAL
    loadMaterial();
    // LOAD LIGHTS
    loadLights();
    
    // Model Matrix
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(tp_camera_->getViewMatrix());
    // Perspective Matrix
    glm::mat4 P(tp_camera_->getPerspectiveMatrix());
    // Draw each object
    for (NodeMapIterator iter = node_map_.begin(); iter != node_map_.end(); ++iter)
    {
        (iter->second)->draw(current_program_iter_->second, M, V, P);
    }
}

/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneNormal::resize(int width, int height)
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
void GLSceneNormal::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '0':
            if(++current_program_iter_ == glsl_program_map_.end())
                current_program_iter_ = glsl_program_map_.begin();
            break;

        case 'a':
        case 'A':
            node_map_["cubes"]->rotateX(-1.0f);
            break;

        case 'd':
        case 'D':
            node_map_["cubes"]->rotateX(1.0f);
            break;

        case 'q':
        case 'Q':
            node_map_["cubes"]->rotateY(1.0f);
            break;

        case 'e':
        case 'E':
            node_map_["cubes"]->rotateY(-1.0f);
            break;

        case 'w':
        case 'W':
            node_map_["cubes"]->rotateZ(-1.0f);
            break;

        case 's':
        case 'S':
            node_map_["cubes"]->rotateZ(1.0f);
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
            tp_camera_->addHeightToTarget(0.1f);
            break;

        case 'l':
        case 'L':
            camera_gps_->rotateX(1.0f); // Pitch
            tp_camera_->addHeightToTarget(-0.1f);
            break;

        case 'u':
        case 'U':
            camera_gps_->translate(glm::vec3(0.0f, 0.0f, -0.1f));
            tp_camera_->addDistanceToTarget(0.1f);
            break;

        case 'j':
        case 'J':
            camera_gps_->translate(glm::vec3(0.0f, 0.0f, 0.1f));
            tp_camera_->addDistanceToTarget(-0.1f);
            break;
    }
}

void GLSceneNormal::mouseClick(int button, int state, int x, int y)
{
    tp_camera_->mouse(button, state, x, y);
}

void GLSceneNormal::cleanup(void)
{
    delete gl_cube_instance_;
    delete gl_cube_;
    delete camera_gps_;
    delete camera_;
    delete gl_sphere_instance_;
    delete gl_sphere_;

    std::map<std::string, Node3D *>::const_iterator iter;
    for (iter = node_map_.begin(); iter != node_map_.end(); ++iter)
    {
        delete iter->second;
    }
}


