/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "GraphicsDefs.hpp"			// TAB_KEY
#include "GLSceneAntTweakBar.hpp"   // GLSceneAntTweakBar
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "CameraInterface.hpp"      // camera_Interface
#include "CameraFirstPerson.hpp"    // camera_FirstPerson
#include "CameraThirdPerson.hpp"    // camera_FirstPerson
#include "ShapeHelper2.hpp"          // build Mesh helper funtions
#include "TextureManager.hpp"       // loadTexture()
#include "DebugGlm.hpp"				// operator<<
#include "Material.hpp"				// MaterialManager

// Global includes
#include <JU/core/Object3D.hpp>     // Object3D
#include <glm/gtx/transform2.hpp>   // glm::rotate, glm::translate
#include <SOIL.h>                   // SOIL_save_image
#include <math.h>					// M_PI
#include <iostream>					// std::printf



GLSceneAntTweakBar::GLSceneAntTweakBar(int width, int height) : GLScene(width, height),
									shadow_map_width_(2048), shadow_map_height_(2048), pcf_enabled_(false),
                                    shadowFBO_(0), depthTex_(0), pass1Index_(0), pass2Index_(0),
									camera_gps_(nullptr), camera_(nullptr), control_camera_(true), camera_controller_(width, height, 0.5f),
									show_bar_(false), pTwBar_(nullptr)
{
}



GLSceneAntTweakBar::~GLSceneAntTweakBar()
{
    // TODO Auto-generated destructor stub
}



/**
* @brief Initialized GLSLProgram objects
*/
void GLSceneAntTweakBar::initializeGLSLPrograms()
{
    glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");

    glsl_program_map_["shadow_mapping"] = compileAndLinkShader("shaders/shadowmap.vs", "shaders/shadowmap.fs");
    glsl_program_map_["shadow_mapping"].setSamplerUniform("shadow_map");

    glsl_program_map_["texture_clip"] = compileAndLinkShader("shaders/texture_clip.vs", "shaders/texture_clip.fs");
    glsl_program_map_["texture_clip"].setSamplerUniform("tex_image");

    current_program_iter_ = glsl_program_map_.find("shadow_mapping");
    current_program_iter_->second.use();

    pass1Index_ = glGetSubroutineIndex(current_program_iter_->second.getHandle(), GL_FRAGMENT_SHADER, "recordDepth");
    pass2Index_ = glGetSubroutineIndex(current_program_iter_->second.getHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");
}



/**
* @brief Initialized the Meshes
*/
void GLSceneAntTweakBar::initializeMeshes()
{
    // GLMESH
    // ------
    Mesh2 mesh;
    GLMesh* pGlMesh;

    ShapeHelper2::buildMesh(mesh, ShapeHelper2::PLANE);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["plane"] = pGlMesh;

    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CUBE);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["cube"] = pGlMesh;

    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CYLINDER, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["cylinder"] = pGlMesh;

    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CONE, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["cone"] = pGlMesh;

    ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 64, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["sphere"] = pGlMesh;

    ShapeHelper2::buildMesh(mesh, ShapeHelper2::TORUS, 64, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["torus"] = pGlMesh;
}



/**
* @brief Initialized the Cameras
*/
void GLSceneAntTweakBar::initializeCameras()
{
	tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 0.5f, 1000.f),
    								   static_cast<Object3D>(*node_map_["main"]),
    								   15.0f, 0.0f, M_PI / 4.0f);
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);
}



/**
* @brief Initialized the Lights
*/
void GLSceneAntTweakBar::initializeLights()
{

}



/**
* @brief Initialized the Lights
*/
void GLSceneAntTweakBar::initializeObjects()
{
    // TEXTURES
    // ----------------------------------------------------------------
    //TextureManager::loadTexture("test", "texture/test.tga");

	// MATERIALS
	// ---------
	MaterialManager::init();

	// MAIN MESH
    // ---------
    Material material;
    GLMeshInstance* mesh_instance;
    std::string material_name("chrome");
    if (!MaterialManager::getMaterial(material_name, material))
    {
    	std::printf("Material %s not found\n", material_name.c_str());
    	exit(EXIT_FAILURE);
    }
    mesh_instance = new GLMeshInstance;
    mesh_instance->setMesh(mesh_map_["sphere"]);
    mesh_instance->setScale(5.0f, 5.0f, 5.0f);
    mesh_instance->setMaterial(&material);
    mesh_instance_map_["main"] = mesh_instance;

    // TORUS
    // ------
    mesh_instance = new GLMeshInstance;
    mesh_instance->setMesh(mesh_map_["torus"]);
    mesh_instance->setScale(5.0f, 5.0f, 5.0f);
    mesh_instance->setMaterial(&material);
    mesh_instance_map_["torus"] = mesh_instance;

    // PLANE
    // ------
    material_name = "green_plastic";
    if (!MaterialManager::getMaterial(material_name, material))
    {
    	std::printf("Material %s not found\n", material_name.c_str());
    	exit(EXIT_FAILURE);
    }
    mesh_instance = new GLMeshInstance;
    mesh_instance->setMesh(mesh_map_["plane"]);
    mesh_instance->setScale(100.0f, 100.0f, 2.0f);
    mesh_instance->setMaterial(&material);
    mesh_instance_map_["plane"] = mesh_instance;


    // LIGHT SPHERE
    mesh_instance = new GLMeshInstance;
    mesh_instance->setMesh(mesh_map_["sphere"]);
    mesh_instance->setScale(0.5f, 0.5f, 0.5f);
    TextureManager::loadTexture("light", "texture/light_texture.tga");
    mesh_instance->addColorTexture("light");
    mesh_instance_map_["light"] = mesh_instance;

    // NODES
    // ----------------------------------------------------------------
    Node3D* pnode = 0;
    // Main
    // ----
    // Give the sphere a position and a orientation
    Object3D main  (glm::vec3(0.0f, 10.0f,  0.0f), // Model's position
                    glm::vec3(1.0f,  0.0f,  0.0f), // Model's X axis
                    glm::vec3(0.0f,  0.0f, -1.0f), // Model's Y axis
                    glm::vec3(0.0f,  1.0f,  0.0f));// Model's Z axis
    pnode = new Node3D(main, mesh_instance_map_["main"], true);
    node_map_["main"] = pnode;

    // Columns
    // ---------
    glm::vec3 ring_center (0.0f, 0.0f, 0.0f);
    JU::f32 radius = 20.0f;
    JU::uint32 num_columns = 10;
    JU::f32 min_height = 5.0f , max_height = 30.0f;
    JU::f32 height_delta = (max_height - min_height) / num_columns;
    JU::f32 height = min_height;
    std::string instance_prefix("cylinder_5_2_");
    JU::f32 angle_delta = 2 * M_PI / num_columns;
    JU::f32 angle = 0.0f;
    for (JU::uint32 index = 0; index < num_columns; ++index)
    {
        // CYLINDER
        // --------
    	JU::f32 scale_y =  height;
        mesh_instance = new GLMeshInstance;
        mesh_instance->setMesh(mesh_map_["cylinder"]);
        mesh_instance->setScale(5.0f, 2.0f, scale_y);
        mesh_instance->setMaterial(&material);
        mesh_instance_map_[instance_prefix + std::to_string(height)] = mesh_instance;

        glm::vec3 position(radius * cosf(angle),
        				   ring_center.y + scale_y * 0.5f,
						   radius * sinf(angle));

		// Give the sphere a position and a orientation
		Object3D column_object (position, // Model's position
								glm::vec3( 1.0f,  0.0f,  0.0f), // Model's X axis
								glm::vec3( 0.0f,  0.0f, -1.0f), // Model's Y axis
								glm::vec3( 0.0f,  1.0f,  0.0f));// Model's Z axis
		pnode = new Node3D(column_object, mesh_instance, true);
		node_map_[std::string("column") + std::to_string(index)] = pnode;

		height += height_delta;
		angle  += angle_delta;
    }

    // Torus
    // ----
    // Give the sphere a position and a orientation
    Object3D torus (glm::vec3( 5.0f, 10.0f,  0.0f), // Model's position
                    glm::vec3( 1.0f,  0.0f,  0.0f), // Model's X axis
                    glm::vec3( 0.0f,  1.0f,  0.0f), // Model's Y axis
                    glm::vec3( 0.0f,  0.0f,  1.0f));// Model's Z axis
    pnode = new Node3D(torus, mesh_instance_map_["torus"], true);
    node_map_["torus"] = pnode;

    // Plane
    // -----
    // Give the plane a position and a orientation
    Object3D plane(glm::vec3(0.0f, 0.0f, 0.0f), // Model's position
                   glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis
                   glm::vec3(0.0f, 0.0f,-1.0f), // Model's Y axis
                   glm::vec3(0.0f, 1.0f, 0.0f));// Model's Z axis
    pnode = new Node3D(plane, mesh_instance_map_["plane"], true);
	node_map_["plane"] = pnode;
}



/**
* @brief Initialized the Lights
*/
void GLSceneAntTweakBar::initializeShadowMap()
{
    // Light
    // -----
    // LIGHT FRUSTUM
    glm::vec3 frustum_position (20.0f, 25.0f, 0.0f);
    glm::vec3 frustum_z = glm::normalize(frustum_position);
    glm::vec3 frustum_x = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), frustum_z));
    glm::vec3 frustum_y = glm::normalize(glm::cross(frustum_z, frustum_x));

    Object3D light_Frustum_3d (frustum_position, // camera_'s position (eye's coordinates)
                               frustum_x, // camera_'s X axis
                               frustum_y, // camera_'s Y axis
                               frustum_z);// VIEWING AXIS (the camera_ is looking into its NEGATIVE Z axis)
    light_frustum_= new CameraFirstPerson(CameraIntrinsic(90.f, (float)shadow_map_width_/shadow_map_height_, 5.f, 200.f), light_Frustum_3d);

    glm::vec3 light_intensity (1.0f, 1.0f, 1.0f);

    Object3D root_sphere(light_frustum_->getPosition(),
                         glm::vec3(1.0f, 0.0f,  0.0f), // Model's X axis
                         glm::vec3(0.0f, 1.0f,  0.0f), // Model's Y axis
                         glm::vec3(0.0f, 0.0f,  1.0f));// Model's Z axis
    Node3D *light_node = new Node3D(root_sphere, mesh_instance_map_["light"], true);

    node_map_["light"] = light_node;


    // SHADOW MAP PLANE
	// ----------------
	gl_plane_shadow_instance_ = new GLMeshInstance(mesh_map_["plane"], 0.3f, 0.3f * width_ / height_, 1.0f);
    // Give the plane a position and a orientation
    Object3D plane2(glm::vec3(-0.8f, -0.65f, 0.0f), // Model's position
                    glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis0
                    glm::vec3(0.0f, 1.0f, 0.0f), // Model's Y axis
                    glm::vec3(0.0f, 0.0f, 1.0f));// Model's Z axis
    shadow_plane_node_ = new Node3D(plane2, gl_plane_shadow_instance_, true);

    setupFBO();
}



/**
* @brief Initialized the Scene
*/
void GLSceneAntTweakBar::initAntTweakBar(void)
{
	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL); // for core profile

	pTwBar_ = TwNewBar("Shadow Mapping with PCF");

    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
    TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color
}



/**
* @brief Initialized the Scene
*/
void GLSceneAntTweakBar::init(void)
{
	// Scene
	initializeGLSLPrograms();
	initializeMeshes();
	initializeObjects();
	initializeLights();
	initializeCameras();
	initializeShadowMap();

	// AntTweakBar
	initAntTweakBar();

	// Set up OpenGL
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE); // enables face culling
    glCullFace(GL_BACK); // tells OpenGL to cull back faces (the sane default setting)
}

/**
* @brief Set up the Frame Buffer Object to render the first pass
*/
void GLSceneAntTweakBar::setupFBO(void)
{
    GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };

    // Create and set up the FBO
    glGenFramebuffers(1, &shadowFBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO_);

    // The depth buffer texture
    glGenTextures(1, &depthTex_);

    // Store the texture handle with TextureManager
    TextureManager::registerTexture("shadow_map", depthTex_);

    glActiveTexture(GL_TEXTURE0 + glsl_program_map_["shadow_mapping"].getSamplerTexUnit("shadow_map"));
    glBindTexture(GL_TEXTURE_2D, depthTex_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_width_,
                 shadow_map_height_, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    if (pcf_enabled_)
    {
        // WITH PCF for anti-aliasing shadow edges
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        // WITHOUT PCF for anti-aliasing shadow edges
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex_, 0);

    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers(1, drawBuffers);

    // Check that our frame buffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	exit(EXIT_FAILURE);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void GLSceneAntTweakBar::loadLights(void) const
{
    //for (LightPositionalIterator iter = lights_positional.begin(); iter != lights_positional.end(); ++iter)
    //{

    // 0
    glm::vec4 light_position (light_frustum_->getPosition(), 1.0f);
    light_position = tp_camera_->getViewMatrix() * light_position;
    (current_program_iter_->second).setUniform("Light.Position",  light_position);
    (current_program_iter_->second).setUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));
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
void GLSceneAntTweakBar::update(float time)
{
    float radius_delta, angle;
    glm::vec3 axis;
    camera_controller_.update(radius_delta, angle, axis);

    NodeMapIterator main_iter = node_map_.find("main");
    if (main_iter == node_map_.end())
    {
    	std::printf("Main Object Node not found\n");
    	exit(EXIT_FAILURE);
    }

    // Use the arcball to control the camera or an object?
    if (control_camera_)
    {
        // Convert the axis from the camera to the world coordinate system
        axis = glm::vec3(tp_camera_->getTransformToParent() * glm::vec4(axis, 0.0f));
        tp_camera_->update(static_cast<const Object3D&>(*main_iter->second), radius_delta, angle, axis);
    }
    else
    {
        axis = glm::vec3(tp_camera_->getTransformToParent() * glm::vec4(-axis, 0.0f));
        main_iter->second->rotate(angle, axis);
    }

	static float delta_theta = M_PI * 0.05f * 0.001f; // radians/millisecond
	glm::vec4 light_position (light_frustum_->getPosition(), 1.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), delta_theta * time, glm::vec3(0.0f, 1.0f, 0.0f));
	light_position = rotation * light_position;

    glm::vec3 light_z = glm::normalize(glm::vec3(light_position.x, light_position.y, light_position.z) - main_iter->second	->getPosition());
    glm::vec3 light_x = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), light_z));
    glm::vec3 light_y = glm::normalize(glm::cross(light_z, light_x));

	light_frustum_->setPosition(glm::vec3(light_position.x, light_position.y, light_position.z));
	light_frustum_->setXAxis(light_x);
	light_frustum_->setYAxis(light_y);
	light_frustum_->setZAxis(light_z);

	node_map_["light"]->setPosition(light_frustum_->getPosition());
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneAntTweakBar::drawScene(const CameraInterface* camera) const
{
	static glm::mat4 bias (0.5f, 0.0f, 0.0f, 0.0f,
						   0.0f, 0.5f, 0.0f, 0.0f,
						   0.0f, 0.0f, 0.5f, 0.0f,
						   0.5f, 0.5f, 0.5f, 1.0f);

	// Model Matrix
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(camera->getViewMatrix());
    // Perspective Matrix
    glm::mat4 P(camera->getPerspectiveMatrix());
    // Shadow Matrix
    glm::mat4 shadow_matrix = bias * light_frustum_->getPerspectiveMatrix() * light_frustum_->getViewMatrix();
    // Draw
    current_program_iter_->second.setUniform("ShadowMatrix", shadow_matrix);

    // Draw each object
    for (NodeMapIterator iter = node_map_.begin(); iter != node_map_.end(); ++iter)
    {
        (iter->second)->draw(current_program_iter_->second, M, V, P);
    }
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneAntTweakBar::render(void)
{
    if(current_program_iter_ == glsl_program_map_.find("perfragment"))
    {
        renderPerfragmentLighting();
    }
    else
    {
        //current_program_iter_->second.use();
        renderShadow();
    }

    /*
    current_program_iter_ = glsl_program_map_.find("shadow_mapping");
    program_iter_->second.use();
    camera_ = fp_camera_;
    renderShadow();
    */

    // AntTweakBar
    if (show_bar_)
    	TwDraw();  // draw the tweak bar(s)
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneAntTweakBar::renderPerfragmentLighting(void) const
{
    // Set up the viewport
    glViewport(0, 0, width_, height_);
    // Bind to the default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Clear both the depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // LOAD MATERIAL
    //loadMaterial();
    // LOAD LIGHTS
    loadLights();

    // Model Matrix
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(camera_->getViewMatrix());
    // Perspective Matrix
    glm::mat4 P(camera_->getPerspectiveMatrix());
    // Draw each object
    drawScene(camera_);

    TextureManager::unbindAllTextures();
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneAntTweakBar::renderShadow(void) const
{
	current_program_iter_->second.use();

	// PASS 1 (RECORD DEPTH)
    // ----------------------------------
    // LOAD LIGHTS
    loadLights();

    // Set up the viewport
    glViewport(0, 0, shadow_map_width_, shadow_map_height_);

    // Bind the frame buffer containing the shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO_);

    // Clear it
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Select the fragment shader subroutine to record the depth
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index_);

    // Enable front-face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Draw the scene
    drawScene(light_frustum_);

    glFlush();
    glFinish();

    //spitOutDepthBuffer();

    // PASS 2 (RENDER THE SCENE)
    // -------------------------
    // LOAD LIGHTS
    loadLights();

    // Set up the viewport
    glViewport(0, 0, width_, height_);
    // Bind to the default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Clear both the depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Activate the shadow map texture

    glActiveTexture(GL_TEXTURE0 + current_program_iter_->second.getSamplerTexUnit("shadow_map"));
    TextureManager::bindTexture("shadow_map");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    // Select the fragment shader subroutine to shade the scene with the shadow map
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index_);
    // Disable culling (we could also switch back to back-face culling)
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Draw the scene
    drawScene(camera_);

    // DRAW PLANE in homogeneous clip coordinates
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(1.0f);
    // Perspective Matrix
    glm::mat4 P(1.0f);
    // Disable depth buffer
    glDisable(GL_DEPTH_TEST);

    // Set the program
    GLSLProgramMapIter iter = glsl_program_map_.find("texture_clip");
    iter->second.use();
    // Bind the shadow map
    glActiveTexture(GL_TEXTURE0 + iter->second.getSamplerTexUnit("tex_image"));
    TextureManager::bindTexture("shadow_map");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    // Render
    shadow_plane_node_->draw(iter->second, M, V, P);

    TextureManager::unbindAllTextures();
}

/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneAntTweakBar::resize(int width, int height)
{
    GLScene::resize(width, height);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    tp_camera_->setAspectRatio(static_cast<float>(width)/height);
    camera_controller_.windowResize(width, height);

    // AntTweakBar
	TwWindowSize(width, height);
}


/**
* @brief Handle keyboard input
*
* @param key    Id of key pressed
* @param x      Location of the mouse when the key was pressed
* @param y      Location of the mouse when the key was pressed
*/
void GLSceneAntTweakBar::keyboard(unsigned char key, int x, int y)
{
	static MeshMapIterator mesh_iter = mesh_map_.find("sphere");

    switch (key)
    {
        case '0':
            if(current_program_iter_ == glsl_program_map_.find("perfragment"))
            {
                current_program_iter_ = glsl_program_map_.find("shadow_mapping");
                std::cout << "Shadow Mapping Enabled" << std::cout;
            }
            else
            {
                current_program_iter_ = glsl_program_map_.find("perfragment");
                std::cout << "Per-fragment Lighting Enabled" << std::cout;
            }

            break;

        case TAB_KEY:
        	show_bar_ = !show_bar_;
        	break;

        case 'c':
        case 'C':
            control_camera_ = !control_camera_;
            break;

        case '1':
            if (camera_ == tp_camera_)
            {
                camera_ = light_frustum_;
                std::cout << "Light Frustum" << std::endl;
            }
            else
            {
                camera_ = tp_camera_;
                std::cout << "First Person Camera" << std::endl;
            }
            break;

        case '2':
        	if (++mesh_iter == mesh_map_.end())
        		mesh_iter = mesh_map_.begin();

        	mesh_instance_map_["main"]->setMesh(mesh_iter->second);
        	break;

        default:
        	std::printf("Unhandled key %c\n", key);
        	break;
    }
}

void GLSceneAntTweakBar::mouseClick(int button, int state, int x, int y)
{
	camera_controller_.mouseClick(button, state, x, y);
}

void GLSceneAntTweakBar::mouseMotion(int x, int y)
{
	camera_controller_.mouseMotion(x, y);
}

void GLSceneAntTweakBar::clear(void)
{
	for (MeshMapIterator iter = mesh_map_.begin(); iter != mesh_map_.end(); ++iter)
		delete iter->second;

	for (MeshInstanceMapIterator iter = mesh_instance_map_.begin(); iter != mesh_instance_map_.end(); ++iter)
		delete iter->second;

    delete camera_gps_;
    delete fp_camera_;
    delete light_frustum_;
    delete gl_plane_shadow_instance_;
    delete shadow_plane_node_;

    // Release AntTweakBar
	TwTerminate();
}

void GLSceneAntTweakBar::spitOutDepthBuffer() const
{
    int size = shadow_map_width_ * shadow_map_height_;
    float * buffer = new float[size];
    unsigned char * imgBuffer = new unsigned char[size * 4];

    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);

    for( int i = 0; i < shadow_map_height_; i++ )
        for( int j = 0; j < shadow_map_width_; j++ )
        {
            int imgIdx = 4 * ((i*shadow_map_width_) + j);
            int bufIdx = ((shadow_map_height_ - i - 1) * shadow_map_width_) + j;

            // This is just to make a more visible image.  Scale so that
            // the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
            // be tweaked for different light configurations.

            /*
            float minVal = 0.88f;
            float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
            unsigned char val = (unsigned char)(scale * 255);
			*/

            unsigned char val = (unsigned char)(buffer[bufIdx] * 255);
            imgBuffer[imgIdx] = val;
            imgBuffer[imgIdx+1] = val;
            imgBuffer[imgIdx+2] = val;
            imgBuffer[imgIdx+3] = 0xff;
        }

    //QImage img(imgBuffer, shadow_map_width_, shadow_map_height_, QImage::Format_RGB32);
    //img.save("depth.png", "PNG");

    int save_result = SOIL_save_image("depth.tga",
    								   SOIL_SAVE_TYPE_TGA,
    								   shadow_map_width_,
    								   shadow_map_height_,
    								   4/* channels*/,
    								   imgBuffer);

    if (save_result == 0)
        std::printf("ERROR: could not save depth texture\n");

    delete [] buffer;
    delete [] imgBuffer;
    exit(1);
}



