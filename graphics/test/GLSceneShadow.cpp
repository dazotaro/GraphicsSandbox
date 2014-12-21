/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

// Local includes
#define _USE_MATH_DEFINES
#include <cmath>
#include "GLSceneShadow.hpp"      // GLSceneShadow
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "CameraInterface.hpp"      // camera_Interface
#include "CameraFirstPerson.hpp"    // camera_FirstPerson
#include "CameraThirdPerson.hpp"    // camera_FirstPerson
#include "ShapeHelper2.hpp"          // build Mesh helper funtions
#include "TextureManager.hpp"       // loadTexture()
#include <glm/gtx/transform2.hpp>   // glm::rotate, glm::translate
#include "DebugGlm.hpp"				// operator<<
#include <SOIL.h>                   // SOIL_save_image
#include "Material.hpp"				// MaterialManager

// Global includes
#include <JU/core/Object3D.hpp>     // Object3D



GLSceneShadow::GLSceneShadow(int width, int height) : GLScene(width, height),
									 camera_gps_(0), camera_(0),
                                     shadow_map_width_(2048), shadow_map_height_(2048), pcf_enabled_(false),
                                     shadowFBO_(0), depthTex_(0), control_camera_(true),
                                     camera_controller_(width, height, 0.5f)
{
}



GLSceneShadow::~GLSceneShadow()
{
    // TODO Auto-generated destructor stub
}



/**
* @brief Initialized GLSLProgram objects
*/
void GLSceneShadow::initializeGLSLPrograms()
{
    glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");

    glsl_program_map_["shadow_mapping"] = compileAndLinkShader("shaders/shadowmap.vs", "shaders/shadowmap.fs");
    glsl_program_map_["shadow_mapping"].setSamplerUniform("shadow_map");

    glsl_program_map_["texture_clip"] = compileAndLinkShader("shaders/texture_clip.vs", "shaders/texture_clip.fs");
    glsl_program_map_["texture_clip"].setSamplerUniform("tex_image");

    current_program_iter_ = glsl_program_map_.find("shadow_mapping");
    current_program_iter_->second.use();
}



/**
* @brief Initialized the Meshes
*/
void GLSceneShadow::initializeMeshes()
{
    // GLMESH
    // ------
    Mesh2 mesh;
    GLMesh* pGlMesh;
    // Plane
    // -----
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::PLANE);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["plane"] = pGlMesh;
    // Cube
    // -----
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CUBE);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["cube"] = pGlMesh;
    // Cylinder
    // -----
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CYLINDER, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["cylinder"] = pGlMesh;
    // Cube
    // -----
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CONE, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["cone"] = pGlMesh;
    // Cube
    // -----
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 64, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["sphere"] = pGlMesh;
    // Cube
    // -----
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::TORUS, 64, 64);
    mesh.computeTangents();
    pGlMesh = new GLMesh(mesh);
    pGlMesh->init();
    mesh_map_["torus"] = pGlMesh;
}



/**
* @brief Initialized the Cameras
*/
void GLSceneShadow::initializeCameras()
{
	tp_camera_ = new CameraThirdPerson(CameraIntrinsic(90.f, width_/(float)height_, 0.5f, 1000.f),
    								   static_cast<Object3D>(*node_map_["main"]),
    								   10.0f, 0.0f, M_PI / 2.0f);
    camera_ = dynamic_cast<CameraInterface *>(tp_camera_);
}



/**
* @brief Initialized the Lights
*/
void GLSceneShadow::initializeLights()
{

}



/**
* @brief Initialized the Lights
*/
void GLSceneShadow::initializeObjects()
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
    mesh_instance->setScale(50.0f, 50.0f, 5.0f);
    mesh_instance->setMaterial(&material);
    mesh_instance_map_["plane"] = mesh_instance;


    // LIGHT SPHERE
    mesh_instance = new GLMeshInstance;
    mesh_instance->setMesh(mesh_map_["sphere"]);
    mesh_instance->setScale(1.0f, 1.0f, 1.0f);
    TextureManager::loadTexture("light", "texture/light_texture.tga");
    mesh_instance->addColorTexture("light");
    mesh_instance_map_["light"] = mesh_instance;

    // NODES
    // ----------------------------------------------------------------
    // Main
    // ----
    // Give the sphere a position and a orientation
    Object3D main  (glm::vec3(0.0f, 10.0f,  0.0f), // Model's position
                    glm::vec3(1.0f,  0.0f,  0.0f), // Model's X axis
                    glm::vec3(0.0f,  0.0f, -1.0f), // Model's Y axis
                    glm::vec3(0.0f,  1.0f,  0.0f));// Model's Z axis
    NodePointerList no_children;
    Node3D* main_node = new Node3D(main, mesh_instance_map_["main"], no_children, true);

    node_map_["main"] = main_node;

    // Torus
    // ----
    // Give the sphere a position and a orientation
    Object3D torus (glm::vec3( 5.0f, 20.0f,  0.0f), // Model's position
                    glm::vec3( 1.0f,  0.0f,  0.0f), // Model's X axis
                    glm::vec3( 0.0f,  0.0f, -1.0f), // Model's Y axis
                    glm::vec3( 0.0f,  1.0f,  0.0f));// Model's Z axis
    Node3D* torus_node = new Node3D(torus, mesh_instance_map_["torus"], no_children, true);

    node_map_["torus"] = torus_node;

    // Plane
    // -----
    // Give the plane a position and a orientation
    Object3D plane(glm::vec3(0.0f, 0.0f, 0.0f), // Model's position
                   glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis
                   glm::vec3(0.0f, 0.0f,-1.0f), // Model's Y axis
                   glm::vec3(0.0f, 1.0f, 0.0f));// Model's Z axis
    Node3D* plane_node = new Node3D(plane, mesh_instance_map_["plane"], no_children, true);

	node_map_["plane"] = plane_node;

}



/**
* @brief Initialized the Lights
*/
void GLSceneShadow::initializeShadowMap()
{
    // Light
    // -----
    // LIGHT FRUSTUM
    glm::vec3 frustum_position (10.0f, 30.0f, 0.0f);
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
    NodePointerList light_children;
    Node3D *light_node = new Node3D(root_sphere, mesh_instance_map_["light"], light_children, true);

    node_map_["light"] = light_node;


    // SHADOW MAP PLANE
	// ----------------
	gl_plane_shadow_instance_ = new GLMeshInstance(mesh_map_["plane"], 0.3f, 0.3f * width_ / height_, 1.0f);
    // Give the plane a position and a orientation
    Object3D plane2(glm::vec3(-0.8f, -0.65f, 0.0f), // Model's position
                    glm::vec3(1.0f, 0.0f, 0.0f), // Model's X axis0
                    glm::vec3(0.0f, 1.0f, 0.0f), // Model's Y axis
                    glm::vec3(0.0f, 0.0f, 1.0f));// Model's Z axis
    NodePointerList no_children;
    shadow_plane_node_ = new Node3D(plane2, gl_plane_shadow_instance_, no_children, true);

    setupFBO();
}



/**
* @brief Initialized the Scene
*/
void GLSceneShadow::init(void)
{
	initializeGLSLPrograms();
	initializeMeshes();
	initializeObjects();
	initializeLights();
	initializeCameras();
	initializeShadowMap();


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
void GLSceneShadow::setupFBO(void)
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

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void GLSceneShadow::loadLights(void) const
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
void GLSceneShadow::update(float time)
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
        main_iter->second->rotate(glm::degrees(angle), axis);
    }

	static float delta_theta = M_PI * 0.02f * 0.001f; // radians/millisecond
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
void GLSceneShadow::drawScene(const CameraInterface* camera) const
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
void GLSceneShadow::render(void)
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
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneShadow::renderPerfragmentLighting(void) const
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
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneShadow::renderShadow(void) const
{
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
    GLuint pass1Index = glGetSubroutineIndex(current_program_iter_->second.getHandle(), GL_FRAGMENT_SHADER, "recordDepth");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);

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
    GLuint pass2Index = glGetSubroutineIndex(current_program_iter_->second.getHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
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
}

/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLSceneShadow::resize(int width, int height)
{
    GLScene::resize(width, height);
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
void GLSceneShadow::keyboard(unsigned char key, int x, int y)
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

        case 'c':
        case 'C':
            control_camera_ = !control_camera_;
            break;

        case 'a':
        case 'A':
            node_map_["sphere"]->rotateX(-1.0f);
            break;

        case 'd':
        case 'D':
            node_map_["sphere"]->rotateX(1.0f);
            break;

        case 'q':
        case 'Q':
            node_map_["sphere"]->rotateY(1.0f);
            break;

        case 'e':
        case 'E':
            node_map_["sphere"]->rotateY(-1.0f);
            break;

        case 'w':
        case 'W':
            node_map_["sphere"]->rotateZ(-1.0f);
            break;

        case 's':
        case 'S':
            node_map_["sphere"]->rotateZ(1.0f);
            break;

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

void GLSceneShadow::mouseClick(int button, int state, int x, int y)
{
	camera_controller_.mouseClick(button, state, x, y);
}

void GLSceneShadow::mouseMotion(int x, int y)
{
	camera_controller_.mouseMotion(x, y);
}

void GLSceneShadow::cleanup(void)
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
}

void GLSceneShadow::spitOutDepthBuffer() const
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



