/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "GLSceneMultipleLights.hpp"      // GLSceneMultipleLights
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "CameraInterface.hpp"      // CameraInterface
#include "CameraFirstPerson.hpp"    // CameraFirstPerson
#include "CameraThirdPerson.hpp"    // CameraThirdPerson
#include "ShapeHelper2.hpp"         // build Mesh helper funtions
#include "TextureManager.hpp"       // loadTexture()
#include "Material.hpp"             // MaterialManager
#include "DebugGlm.hpp"             // debug::print

// Global includes
#include <JU/core/Object3D.hpp>     // Object3D
#include <glm/gtx/transform.hpp>	// glm::rotate
#include <math.h>					// M_PI


GLSceneMultipleLights::GLSceneMultipleLights(int width, int height) : GLScene(width, height),
									 gl_sphere_(0), gl_sphere_instance_(0),
                                     gl_plane_(0), gl_plane_instance_(0),
                                     camera_gps_(0), camera_(0), control_camera_(true),
                                     camera_controller_(width, height, 0.2f)
{
}



GLSceneMultipleLights::~GLSceneMultipleLights()
{
    // TODO Auto-generated destructor stub
}



/**
* @brief Initialized the Scene
*/
void GLSceneMultipleLights::init(void)
{
    //glsl_program_map_["multilight"]  = compileAndLinkShader("shaders/multilight.vert", "shaders/multilight.frag");
    glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");
    //glsl_program_map_["perfragment_halfway"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag_halfway.fs");
    glsl_program_map_["perfragment_texture"] = compileAndLinkShader("shaders/perfrag_texture.vs", "shaders/perfrag_texture.fs");
    glsl_program_map_["normal_drawing"] = compileAndLinkShader("shaders/normal_drawing.vs",
                                                               "shaders/normal_drawing.gs",
                                                               "shaders/simple.frag");

    glsl_program_map_["ntb_drawing"] = compileAndLinkShader("shaders/normal_drawing.vs",
                                                            "shaders/ntb_drawing.gs",
                                                            "shaders/simple.frag");

    glsl_program_map_["normal_drawing_face"] = compileAndLinkShader("shaders/normal_drawing.vs",
                                                                    "shaders/normal_drawing_face.gs",
                                                                    "shaders/simple.frag");

    glsl_program_map_["wireframe"] = compileAndLinkShader("shaders/wireframe.vs",
                                                          "shaders/wireframe.gs",
                                                          "shaders/simple.frag");

    current_program_iter_ = glsl_program_map_.find("perfragment_texture");
    
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)

    // TEXTURES
    // --------
    TextureManager::loadTexture("test",  "texture/test.tga");
    TextureManager::loadTexture("brick", "texture/brick1.jpg");
    TextureManager::loadTexture("pool",  "texture/pool.png");
    TextureManager::loadTexture("light", "texture/light_texture.tga");

    // MATERIALS
    // ---------
    MaterialManager::init();
    Material mat_sphere;
    if (!MaterialManager::getMaterial("ruby", mat_sphere))
        exit(EXIT_FAILURE);
    Material mat_plane;
    if (!MaterialManager::getMaterial("green_rubber", mat_plane))
        exit(EXIT_FAILURE);

     // SPHERE
    // ------
    // Create Mesh
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 64, 32);
    mesh.computeTangents();
    gl_sphere_ = new GLMesh(mesh);
    // Load the Mesh into VBO and VAO
    gl_sphere_->init();
    // Create instance of GLMesh (there could be more than one)
    gl_sphere_instance_ = new GLMeshInstance(gl_sphere_, 5.0f, 5.0f, 5.0f, &mat_sphere);
    gl_sphere_instance_->addColorTexture("pool");
    // Give the sphere a position and a orientation
    Object3D sphere(glm::vec3(0.0f, 10.0f,  0.0f), // Model's position
                    glm::vec3(1.0f,  0.0f,  0.0f), // Model's X axis
                    glm::vec3(0.0f,  0.0f, -1.0f), // Model's Y axis
                    glm::vec3(0.0f,  1.0f,  0.0f));// Model's Z axis
    NodePointerList no_children;
    Node3D* sphere_node = new Node3D(sphere, gl_sphere_instance_, no_children, true);

	node_map_["sphere"] = sphere_node;

    // SPHERE
    // ------
    // Create Mesh
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 64, 32);
    //mesh.computeTangents();
    gl_sphere_ = new GLMesh(mesh);
    // Load the Mesh into VBO and VAO
    gl_sphere_->init();

    // PLANE
    // ------
    // Create Mesh
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::PLANE);
    gl_plane_ = new GLMesh(mesh);
    // Load the Mesh into VBO and VAO
    gl_plane_->init();
    // Create instance of GLMEsh (there could be more than one)
    gl_plane_instance_ = new GLMeshInstance(gl_plane_, 50.0f, 50.0f, 1.0f, &mat_plane);
    gl_plane_instance_->addColorTexture("brick");
    // Give the plane a position and a orientation
    Object3D plane(glm::vec3(0.0f, 0.0f, 0.0f), // Model's position
                   glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis
                   glm::vec3(0.0f, 0.0f,-1.0f), // Model's Y axis
                   glm::vec3(0.0f, 1.0f, 0.0f));// Model's Z axis
    Node3D* plane_node = new Node3D(plane, gl_plane_instance_, no_children, true);

	node_map_["plane"] = plane_node;


    // Create the Camera    // Create the camera_
    glm::vec3 camera_position (0.0f, 20.0f, 10.0f);
    glm::vec3 camera_z = glm::normalize(camera_position);
    glm::vec3 camera_x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_z));
    glm::vec3 camera_y = glm::normalize(glm::cross(camera_z, camera_x));
    camera_gps_ = new Object3D(camera_position, // camera_'s position (eye's coordinates)
                               camera_x, // camera_'s X axis
                               camera_y, // camera_'s Y axis
                               camera_z);// VIEWING AXIS (the camera_ is looking into its NEGATIVE Z axis)
    //fp_camera_ = new CameraFirstPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), *camera_gps_);
    tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 0.5f, 1000.f),
    								   static_cast<Object3D>(*sphere_node),
    								   10.0f, 0.0f, M_PI / 2.0f);
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);

    /*
    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * vec4(5.0f,5.0f,2.0f,1.0f) );
    */

    // LIGHTS
    //---------
    glm::vec3 light_position (0.0f, 20.0f, 10.0f);
    glm::vec3 light_intensity (1.0f, 1.0f, 1.0f);
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

void GLSceneMultipleLights::loadMaterial(void) const
{
    (current_program_iter_->second).setUniform("Kd", 0.8f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Ks", 0.9f, 0.9f, 0.9f);
    (current_program_iter_->second).setUniform("Ka", 0.1f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Shininess", 10.0f);
}



void GLSceneMultipleLights::loadLights(void) const
{
    // WARNING: The shader expects the light position in eye coordinates
	(current_program_iter_->second).setUniform("Light.Position",  tp_camera_->getViewMatrix() * glm::vec4(lights_positional_[0].position_,1.0f));
    (current_program_iter_->second).setUniform("Light.Intensity", lights_positional_[0].intensity_);
}



/**
* @brief Update everything that needs to be updated in the scene
*
* @param time Time elapsed since the last update (in milliseconds)
*/
void GLSceneMultipleLights::update(float time)
{
    float radius_delta, angle;
    glm::vec3 axis;
    camera_controller_.update(radius_delta, angle, axis);

    // Use the arcball to control the camera or an object?
    if (control_camera_)
    {
        // Convert the axis from the camera to the world coordinate system
        axis = glm::vec3(tp_camera_->getTransformToParent() * glm::vec4(axis, 0.0f));
        tp_camera_->update(static_cast<const Object3D&>(*node_map_["sphere"]), radius_delta, angle, axis);
    }
    else
    {
        axis = glm::vec3(tp_camera_->getTransformToParent() * glm::vec4(-axis, 0.0f));
        node_map_["sphere"]->rotate(glm::degrees(angle), axis);
    }

	// LIGHTS: update position
    static const float angle_speed = (2.0 * M_PI * 0.1f) * 0.001f ; // 10 seconds to complete a revolution

    glm::mat4 rotation = glm::rotate(glm::mat4(1.f), angle_speed * time, glm::vec3(0.0f, 1.0f, 0.0f));
    for (LightPositionalVector::iterator light = lights_positional_.begin(); light != lights_positional_.end(); ++light)
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
void GLSceneMultipleLights::render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    current_program_iter_->second.use();

    // LOAD LIGHTS
    loadLights();
    
    //TextureManager::bindTexture(current_program_iter_->second, "normal_map", "NormalMapTex");
    //TextureManager::bindTexture(current_program_iter_->second, "test", "ColorTex0");

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

    TextureManager::unbindAllTextures();
}



/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneMultipleLights::resize(int width, int height)
{
    GLScene::resize(width, height);
    camera_->setAspectRatio(static_cast<float>(width)/height);
    camera_controller_.windowResize(width, height);
}



/**
* @brief Handle keyboard input
*
* @param key    Id of key pressed
* @param x      Location of the mouse when the key was pressed
* @param y      Location of the mouse when the key was pressed
*/
void GLSceneMultipleLights::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '0':
            if(++current_program_iter_ == glsl_program_map_.end())
                current_program_iter_ = glsl_program_map_.begin();
            break;

        case 'c':
        case 'C':
            control_camera_ = !control_camera_;
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

        /*
        case 'k':
        case 'K':
            fp_camera_->rotateY(1.0f);  // Yaw
            break;

        case ';':
        case ':':
            fp_camera_->rotateY(-1.0f);  // Yaw
            break;

        case 'i':
        case 'I':
            fp_camera_->rotateZ(1.0f);  // Roll
            break;

        case 'p':
        case 'P':
            fp_camera_->rotateZ(-1.0f); // Roll
            break;

        case 'o':
        case 'O':
            fp_camera_->rotateX(-1.0f);  // Pitch
            break;

        case 'l':
        case 'L':
            fp_camera_->rotateX(1.0f); // Pitch
            break;

        case 'u':
        case 'U':
            fp_camera_->translate(glm::vec3(0.0f, 0.0f, -0.1f));
            break;

        case 'j':
        case 'J':
            fp_camera_->translate(glm::vec3(0.0f, 0.0f, 0.1f));
            break;
        */
    }
}



void GLSceneMultipleLights::mouseClick(int button, int state, int x, int y)
{
	camera_controller_.mouseClick(button, state, x, y);
}



void GLSceneMultipleLights::mouseMotion(int x, int y)
{
	camera_controller_.mouseMotion(x, y);
}



void GLSceneMultipleLights::cleanup(void)
{
    delete gl_sphere_;
    delete gl_sphere_instance_;
    delete gl_plane_;
    delete gl_plane_instance_;
    delete camera_gps_;
    //delete fp_camera_;
    delete tp_camera_;

    std::map<std::string, Node3D *>::const_iterator iter;
    for (iter = node_map_.begin(); iter != node_map_.end(); ++iter)
    {
        delete iter->second;
    }
}


