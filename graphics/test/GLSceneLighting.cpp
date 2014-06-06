/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "GLSceneLighting.hpp"      // GLSceneLighting
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "Object3D.hpp"             // Object3D
#include "CameraInterface.hpp"      // camera_Interface
#include "CameraFirstPerson.hpp"    // CameraFirstPerson
#include "CameraThirdPerson.hpp"    // CameraThirdPerson
#include "ShapeHelper2.hpp"          // build Mesh helper funtions
#include "TextureManager.hpp"       // loadTexture()



GLSceneLighting::GLSceneLighting(int width, int height) : GLScene(width, height),
									 gl_sphere_(0), gl_sphere_instance_(0),
                                     gl_plane_(0), gl_plane_instance_(0),
                                     sphere_node_(0), plane_node_(0),
                                     camera_gps_(0), camera_(0),
                                     camera_controller_(width, height, M_PI/4.0f, M_PI/4.0f, 0.2f)
{
}



GLSceneLighting::~GLSceneLighting()
{
    // TODO Auto-generated destructor stub
}



/**
* @brief Initialized the Scene
*/
void GLSceneLighting::init(void)
{
    //glsl_program_map_["multilight"]  = compileAndLinkShader("shaders/multilight.vert", "shaders/multilight.frag");
    glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");
    //glsl_program_map_["perfragment_halfway"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag_halfway.fs");
    //glsl_program_map_["perfragment_texture"] = compileAndLinkShader("shaders/perfrag_texture.vs", "shaders/perfrag_texture.fs");
    glsl_program_map_["normal_drawing"] = compileAndLinkShader("shaders/normal_drawing.vs",
                                                               "shaders/normal_drawing.gs",
                                                               "shaders/simple.frag");

    current_program_iter_ = glsl_program_map_.find("perfragment");
    
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)

    // TEXTURES
    // --------
    //TextureManager::loadTexture("test", "texture/test.tga");

    // SPHERE
    // ------
    // Create Mesh
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 64, 32);
    gl_sphere_ = new GLMesh(mesh);
    // Load the Mesh into VBO and VAO
    gl_sphere_->init();
    // Create instance of GLMEsh (there could be more than one)
    gl_sphere_instance_ = new GLMeshInstance(gl_sphere_, 5.0f, 5.0f, 5.0f);
    gl_sphere_instance_->addColorTexture("test");
    // Give the sphere a position and a orientation
    Object3D sphere(glm::vec3(0.0f, 10.0f, 0.0f), // Model's position
                     glm::vec3(1.0f, 0.0f,  0.0f), // Model's X axis
                     glm::vec3(0.0f, 1.0f,  0.0f), // Model's Y axis
                     glm::vec3(0.0f, 0.0f,  1.0f));// Model's Z axis
    NodePointerList no_children;
    sphere_node_ = new Node3D(sphere, gl_sphere_instance_, no_children, true);

    // PLANE
    // ------
    // Create Mesh
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::PLANE);
    gl_plane_ = new GLMesh(mesh);
    // Load the Mesh into VBO and VAO
    gl_plane_->init();
    // Create instance of GLMEsh (there could be more than one)
    gl_plane_instance_ = new GLMeshInstance(gl_plane_, 50.0f, 50.0f, 1.0f);
    gl_plane_instance_->addColorTexture("test");
    // Give the plane a position and a orientation
    Object3D plane(glm::vec3(0.0f, 0.0f, 0.0f), // Model's position
                   glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis
                   glm::vec3(0.0f, 0.0f,-1.0f), // Model's Y axis
                   glm::vec3(0.0f, 1.0f, 0.0f));// Model's Z axis
    plane_node_ = new Node3D(plane, gl_plane_instance_, no_children, true);

    // Create the camera_
    glm::vec3 camera_position (0.0f, 20.0f, 10.0f);
    glm::vec3 camera_z = glm::normalize(camera_position);
    glm::vec3 camera_x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_z));
    glm::vec3 camera_y = glm::normalize(glm::cross(camera_z, camera_x));
    camera_gps_ = new Object3D(camera_position, // camera_'s position (eye's coordinates)
                               camera_x, // camera_'s X axis
                               camera_y, // camera_'s Y axis
                               camera_z);// VIEWING AXIS (the camera_ is looking into its NEGATIVE Z axis)
    //fp_camera_ = new CameraFirstPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), *camera_gps_);
    tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), static_cast<Object3D>(*sphere_node_));
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);

    /*
    prog.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    prog.setUniform("Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("LightPosition", view * vec4(5.0f,5.0f,2.0f,1.0f) );
    */

    // LIGHTS
    lights_positional_.push_back(LightPositional(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f)));

}



void GLSceneLighting::loadMaterial(void) const
{
    (current_program_iter_->second).setUniform("Kd", 0.8f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Ks", 0.9f, 0.9f, 0.9f);
    (current_program_iter_->second).setUniform("Ka", 0.1f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Shininess", 10.0f);
}



void GLSceneLighting::loadLights(void) const
{
    //for (LightPositionalIterator iter = lights_positional.begin(); iter != lights_positional.end(); ++iter)
    //{

    // 0
    glm::vec4 light_position (camera_gps_->getPosition(), 1.0f);
    light_position = camera_->getViewMatrix() * light_position;
    (current_program_iter_->second).setUniform("Light.Position",  light_position);
    (current_program_iter_->second).setUniform("Light.Intensity", glm::vec3(0.8f,0.8f,0.8f));
    /*
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
* @param time Time elapsed since the last update
*/
void GLSceneLighting::update(float time)
{
    // camera_: update position and orientation
    //camera_->update(*camera_gps_);
    // LIGHTS: update position

	float radius_delta, inclination_delta, azimuth_delta;
	camera_controller_.update(radius_delta, inclination_delta, azimuth_delta);

	tp_camera_->update(static_cast<const Object3D&>(*sphere_node_),
	                   radius_delta,
	                   inclination_delta,
	                   azimuth_delta);

}



/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneLighting::render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    current_program_iter_ = glsl_program_map_.find("perfragment");
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
    (current_program_iter_->second).setUniform("Ka", 0.0f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Kd", 0.0f, 1.0f, 0.1f);
    sphere_node_->draw(current_program_iter_->second, M, V, P);
    (current_program_iter_->second).setUniform("Ka", 0.2f, 0.2f, 0.2f);
    (current_program_iter_->second).setUniform("Kd", 0.2f, 0.2f, 0.2f);
    plane_node_->draw(current_program_iter_->second, M, V, P);

    current_program_iter_ = glsl_program_map_.find("normal_drawing");
    current_program_iter_->second.use();
    sphere_node_->draw(current_program_iter_->second, M, V, P);
    plane_node_->draw(current_program_iter_->second, M, V, P);
}



/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneLighting::resize(int width, int height)
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
void GLSceneLighting::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '0':
            if(++current_program_iter_ == glsl_program_map_.end())
                current_program_iter_ = glsl_program_map_.begin();
            break;

        case 'a':
        case 'A':
            sphere_node_->rotateX(-1.0f);
            break;

        case 'd':
        case 'D':
            sphere_node_->rotateX(1.0f);
            break;

        case 'q':
        case 'Q':
            sphere_node_->rotateY(1.0f);
            break;

        case 'e':
        case 'E':
            sphere_node_->rotateY(-1.0f);
            break;

        case 'w':
        case 'W':
            sphere_node_->rotateZ(-1.0f);
            break;

        case 's':
        case 'S':
            sphere_node_->rotateZ(1.0f);
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



void GLSceneLighting::mouseClick(int button, int state, int x, int y)
{
	camera_controller_.mouseClick(button, state, x, y);
}



void GLSceneLighting::mouseMotion(int x, int y)
{
	camera_controller_.mouseMotion(x, y);
}



void GLSceneLighting::cleanup(void)
{
    delete gl_sphere_;
    delete gl_sphere_instance_;
    delete gl_plane_;
    delete gl_plane_instance_;
    delete sphere_node_;
    delete plane_node_;
    delete camera_gps_;
    //delete fp_camera_;
    delete tp_camera_;
}


