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
#include "GLSLProgramExt.hpp"		// GLSLProgramExt::setUniform

// Global includes
#include <JU/core/Object3D.hpp>     // Object3D
#include <glm/gtx/transform.hpp>	// glm::rotate
#include <math.h>					// M_PI


GLSceneMultipleLights::GLSceneMultipleLights(int width, int height) : GLScene(width, height),
									 camera_(0), control_camera_(true), camera_controller_(width, height, 0.2f),
									 num_pos_lights_(1)
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
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)

    initializePrograms();
    initializeMaterials();
    initializeTextures();
    initializeObjects();
    initializeCameras();
    initializeLights();
}



void GLSceneMultipleLights::initializePrograms()
{
    glsl_program_map_["multilight"]  = compileAndLinkShader("shaders/multilight.vs", "shaders/multilight.fs");
    glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");
    glsl_program_map_["perfragment_texture"] = compileAndLinkShader("shaders/perfrag_texture.vs", "shaders/perfrag_texture.fs");

    current_program_iter_ = glsl_program_map_.find("multilight");
}



void GLSceneMultipleLights::initializeMaterials()
{
	Material* pmat;

    MaterialManager::init();

    pmat = new Material;
    if (!MaterialManager::getMaterial("ruby", *pmat))
        exit(EXIT_FAILURE);
    material_map_["ruby"] = pmat;

    pmat = new Material;
    if (!MaterialManager::getMaterial("gray_rubber", *pmat))
        exit(EXIT_FAILURE);
    material_map_["gray_rubber"] = pmat;

    pmat = new Material;
    if (!MaterialManager::getMaterial("pearl", *pmat))
        exit(EXIT_FAILURE);
    material_map_["pearl"] = pmat;

    pmat = new Material(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    material_map_["white_light"] = pmat;
}



void GLSceneMultipleLights::initializeTextures()
{
    TextureManager::loadTexture("test",  "texture/test.tga");
    TextureManager::loadTexture("brick", "texture/brick1.jpg");
    TextureManager::loadTexture("pool",  "texture/pool.png");
    TextureManager::loadTexture("light", "texture/light_texture.tga");
}



void GLSceneMultipleLights::initializeObjects()
{
	GLMesh* 		pmesh;
	GLMeshInstance* pmesh_instance;
	Node3D*			pnode;

	// SPHERE
	// ------
	// MESH
	Mesh2 mesh;
	ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 64, 32);
	mesh.computeTangents();
	pmesh = new GLMesh(mesh);
	// Load the Mesh into VBO and VAO
	pmesh->init();
	mesh_map_["sphere_64_32"] = pmesh;
	// MESH INSTANCE
	pmesh_instance = new GLMeshInstance(pmesh, 5.0f, 5.0f, 5.0f, material_map_["ruby"]);
	pmesh_instance->addColorTexture("pool");
	mesh_instance_map_["sphere_ruby"] = pmesh_instance;
	// NODE
	// Give the sphere a position and a orientation
	Object3D sphere(glm::vec3(0.0f, 10.0f,  0.0f), // Model's position
				   glm::vec3(1.0f,  0.0f,  0.0f), // Model's X axis
				   glm::vec3(0.0f,  0.0f, -1.0f), // Model's Y axis
				   glm::vec3(0.0f,  1.0f,  0.0f));// Model's Z axis
	pnode = new Node3D(sphere, pmesh_instance, true);
	node_map_["sphere"] = pnode;

    // PLANE
    // ------
    // MESH
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::PLANE);
    pmesh = new GLMesh(mesh);
    // Load the Mesh into VBO and VAO
    pmesh->init();
    mesh_map_["plane"] = pmesh;
    // MESH INSTANCE
    pmesh_instance = new GLMeshInstance(pmesh, 50.0f, 50.0f, 1.0f, material_map_["gray_rubber"]);
    pmesh_instance->addColorTexture("brick");
    mesh_instance_map_["plane_green"];
    // NODE
    // Give the plane a position and a orientation
    Object3D plane(glm::vec3(0.0f, 0.0f, 0.0f), // Model's position
                   glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis
                   glm::vec3(0.0f, 0.0f,-1.0f), // Model's Y axis
                   glm::vec3(0.0f, 1.0f, 0.0f));// Model's Z axis
    pnode = new Node3D(plane, pmesh_instance, true);
	node_map_["plane"] = pnode;

}



void GLSceneMultipleLights::initializeCameras()
{
    tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(JU::f32)height_, 0.5f, 1000.f),
    								   static_cast<Object3D>(*node_map_["sphere"]),
    								   15.0f, 0.0f, M_PI / 4.0f);
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);
}



void GLSceneMultipleLights::initializeLights()
{
	JU::f32 radius = 10.0f;
	JU::f32 channel_intensity = 1.0f / num_pos_lights_;

    glm::vec3 light_ring_center (0.0f, 20.0f, 0.0f);
    glm::vec3 light_intensity (channel_intensity);
    // Create instance of GLMEsh (there could be more than one)
    GLMeshInstance* pmesh_instance = new GLMeshInstance(mesh_map_["sphere_64_32"],		// mesh
    													0.5f, 0.5f, 0.5f,				// scale
														material_map_["white_light"]);	// material
    // Color texture for light object
    //pmesh_instance->addColorTexture("light");
    mesh_instance_map_["light_sphere"] = pmesh_instance;

    lights_positional_.resize(num_pos_lights_, LightPositional(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	JU::f32 angle_delta = 2.0f * M_PI / num_pos_lights_;
    for (JU::uint8 index = 0; index < num_pos_lights_; ++index)
    {
    	JU::f32 angle = index * angle_delta;
    	JU::f32 z = radius * cosf(angle);
    	JU::f32 x = radius * sinf(angle);
        Object3D light_pos(glm::vec3(x, 20.f, z),
                           glm::vec3(1.0f, 0.0f,  0.0f), // Model's X axis
    					   glm::vec3(0.0f, 1.0f,  0.0f), // Model's Y axis
    					   glm::vec3(0.0f, 0.0f,  1.0f));// Model's Z axis

        Node3D *pnode = new Node3D(light_pos, pmesh_instance, true);

        node_map_[std::string("light_pos") + std::to_string(index)] = pnode;

        lights_positional_[index] = (LightPositional(glm::vec3(x, 20.f, z), light_intensity));
    }
}



void GLSceneMultipleLights::loadLights(void) const
{
	glm::mat4 view_matrix = tp_camera_->getViewMatrix();

    // WARNING: The shader expects the light position in eye coordinates
	//(current_program_iter_->second).setUniform("Light.Position",  tp_camera_->getViewMatrix() * glm::vec4(lights_positional_[0].position_,1.0f));
    //(current_program_iter_->second).setUniform("Light.Intensity", lights_positional_[0].intensity_);

	LightPositionalVector eye_lights;
	eye_lights.reserve(lights_positional_.size());
    JU::uint32 index = 0;
    for (LightPositionalVector::const_iterator light = lights_positional_.begin(); light != lights_positional_.end(); ++light)
    {
    	glm::vec4 eye_position = view_matrix * glm::vec4(light->position_, 1.0f);
    	eye_lights.push_back(LightPositional(glm::vec3(eye_position), light->intensity_));
        /*
    	eye_lights[index].position_.x = eye_position.x;
        eye_lights[index].position_.y = eye_position.y;
        eye_lights[index].position_.z = eye_position.z;
        eye_lights[index].intensity_ = light->intensity_;
        */

        ++index;
    }

    GLSLProgramExt::setUniform(current_program_iter_->second, eye_lights);

}



void GLSceneMultipleLights::updateCamera(JU::f32 time)
{
    JU::f32 radius_delta, angle;
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

}



void GLSceneMultipleLights::updateLights(JU::f32 time)
{
	// LIGHTS: update position
    static const JU::f32 angle_speed = (2.0 * M_PI * 0.1f) * 0.001f ; // 10 seconds to complete a revolution

    glm::mat4 rotation = glm::rotate(glm::mat4(1.f), angle_speed * time, glm::vec3(0.0f, 1.0f, 0.0f));
    JU::uint32 index = 0;
    for (LightPositionalVector::iterator light = lights_positional_.begin(); light != lights_positional_.end(); ++light)
    {
        glm::vec4 position = rotation * glm::vec4(light->position_, 0.0f);
        light->position_.x = position.x;
        light->position_.y = position.y;
        light->position_.z = position.z;

        node_map_[std::string("light_pos") + std::to_string(index)]->setPosition(light->position_);

        ++index;
    }
}




/**
* @brief Update everything that needs to be updated in the scene
*
* @param time Time elapsed since the last update (in milliseconds)
*/
void GLSceneMultipleLights::update(JU::f32 time)
{
	updateCamera(time);
	updateLights(time);
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
    for (NodeMap::const_iterator iter = node_map_.begin(); iter != node_map_.end(); ++iter)
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
    camera_->setAspectRatio(static_cast<JU::f32>(width)/height);
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

    		// RELOAD
            case 'l':
				if (num_pos_lights_ < MAX_POS_LIGHTS)
					++num_pos_lights_;
				clear();
            	init();
            	break;

            case 'L':
            	if (num_pos_lights_ > 0)
            		--num_pos_lights_;
            	clear();
            	init();
            	break;

            // RELOAD
			case 'r':
			case 'R':
				clear();
				init();
				break;

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



void GLSceneMultipleLights::clear(void)
{
    delete tp_camera_;

    // GLSLProgram map
    glsl_program_map_.clear();

    // GLMesh map
    for (MeshMap::iterator iter = mesh_map_.begin(); iter != mesh_map_.end(); ++iter)
    {
    	delete iter->second;
    }
    mesh_map_.clear();

    // GLMeshInstance map
    for (MeshInstanceMap::iterator iter = mesh_instance_map_.begin(); iter != mesh_instance_map_.end(); ++iter)
    {
    	delete iter->second;
    }
    mesh_instance_map_.clear();

    // Material map
    for (MaterialMap::const_iterator iter = material_map_.begin(); iter != material_map_.end(); ++iter)
    {
    	delete iter->second;
    }
    material_map_.clear();

    // Node Map
    std::map<std::string, Node3D *>::const_iterator iter;
    for (iter = node_map_.begin(); iter != node_map_.end(); ++iter)
    {
        delete iter->second;
    }
    node_map_.clear();

    // Lights
    lights_positional_.clear();
    lights_directional_.clear();
    lights_spotlight_.clear();
}


