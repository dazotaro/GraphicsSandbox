/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "GLSceneShadow.hpp"      // GLSceneShadow
#include "GLMesh.hpp"               // GLMesh
#include "GLMeshInstance.hpp"       // GLMeshInstance
#include "Node3D.hpp"               // Node3D
#include "Object3D.hpp"             // Object3D
#include "CameraInterface.hpp"      // camera_Interface
#include "CameraFirstPerson.hpp"    // camera_FirstPerson
#include "ShapeHelper.hpp"          // build Mesh helper funtions
#include "TextureManager.hpp"       // loadTexture()
#include <glm/gtx/transform2.hpp>   // glm::rotate, glm::translate
#include "DebugGlm.hpp"				// operator<<
#include <SOIL.h>                   // SOIL_save_image

GLSceneShadow::GLSceneShadow(int width, int height) : GLScene(width, height),
									 gl_sphere_(0), gl_sphere_instance_(0), gl_plane_(0), gl_plane_instance_(0),
                                     sphere_node_(0), plane_node_(0), camera_gps_(0), camera_(0),
                                     shadow_map_width_(2048), shadow_map_height_(2048), pcf_enabled_(false),
                                     shadowFBO_(0), depthTex_(0),
                                     camera_controller_(width, height, M_PI/4.0f, M_PI/4.0f, 10.0f)
{
}

GLSceneShadow::~GLSceneShadow()
{
    // TODO Auto-generated destructor stub
}

/**
* @brief Initialized the Scene
*/
void GLSceneShadow::init(void)
{
    //glsl_program_map_["multilight"]  = compileAndLinkShader("shaders/multilight.vert", "shaders/multilight.frag");
    glsl_program_map_["perfragment"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag.fs");
    glsl_program_map_["shadow_mapping"] = compileAndLinkShader("shaders/shadowmap.vs", "shaders/shadowmap.fs");
    //glsl_program_map_["perfragment_halfway"] = compileAndLinkShader("shaders/perfrag.vs", "shaders/perfrag_halfway.fs");
    //glsl_program_map_["perfragment_texture"] = compileAndLinkShader("shaders/perfrag_texture.vs", "shaders/perfrag_texture.fs");

    current_program_iter_ = glsl_program_map_.find("shadow_mapping");
    
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
    gl_sphere_ = new GLMesh(Graphics::buildMesh(Graphics::SPHERE, 64, 32));
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
    gl_plane_ = new GLMesh(Graphics::buildMesh(Graphics::PLANE));
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
    fp_camera_ = new CameraFirstPerson(CameraIntrinsic(90.f, width_/(float)height_, 1.f, 1000.f), *camera_gps_);
    //camera_ = new camera_ThirdPerson(camera_Intrinsic(90.f, WIDTH/(float)HEIGHT, 1.f, 1000.f), dynamic_cast<Object3D *>(sphere_node_));


    // LIGHT FRUSTUM
    glm::vec3 light_position (10.0f, 30.0f, 0.0f);
    glm::vec3 light_z = glm::normalize(light_position);
    glm::vec3 light_x = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), light_z));
    glm::vec3 light_y = glm::normalize(glm::cross(light_z, light_x));

    Object3D light_Frustum_3d (light_position, // camera_'s position (eye's coordinates)
                               light_x, // camera_'s X axis
                               light_y, // camera_'s Y axis
                               light_z);// VIEWING AXIS (the camera_ is looking into its NEGATIVE Z axis)
    light_frustum_= new CameraFirstPerson(CameraIntrinsic(90.f, (float)shadow_map_width_/shadow_map_height_, 1.f, 100.f), light_Frustum_3d);

    camera_ = fp_camera_;

    // LIGHTS
    lights_positional_.push_back(LightPositional(light_position, glm::vec3(0.8f, 0.8f, 0.8f)));

    setupFBO();

}

/**
* @brief Set up the Frame Buffer Object to render the first pass
*/
void GLSceneShadow::setupFBO(void)
{
    GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
    // The depth buffer texture
    glGenTextures(1, &depthTex_);
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

    // Assign the depth buffer texture to texture channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex_);

    // Create and set up the FBO
    glGenFramebuffers(1, &shadowFBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO_);
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

void GLSceneShadow::loadMaterial(void) const
{
    (current_program_iter_->second).setUniform("Kd", 0.8f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Ks", 0.9f, 0.9f, 0.9f);
    (current_program_iter_->second).setUniform("Ka", 0.1f, 0.1f, 0.1f);
    (current_program_iter_->second).setUniform("Shininess", 10.0f);
}

void GLSceneShadow::loadLights(void) const
{
    //for (LightPositionalIterator iter = lights_positional.begin(); iter != lights_positional.end(); ++iter)
    //{

    // 0
    glm::vec4 light_position (light_frustum_->getPosition(), 1.0f);
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
void GLSceneShadow::update(float time)
{
	static float delta_theta = M_PI * 0.05f;
	glm::vec4 light_position (light_frustum_->getPosition(), 1.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), delta_theta * time, glm::vec3(0.0f, 1.0f, 0.0f));
	light_position = rotation * light_position;

    glm::vec3 light_z = glm::normalize(glm::vec3(light_position.x, light_position.y, light_position.z));
    glm::vec3 light_x = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), light_z));
    glm::vec3 light_y = glm::normalize(glm::cross(light_z, light_x));

	light_frustum_->setPosition(glm::vec3(light_position.x, light_position.y, light_position.z));
	light_frustum_->setXAxis(light_x);
	light_frustum_->setYAxis(light_y);
	light_frustum_->setZAxis(light_z);

	// camera_: update position and orientation
    //camera_->update(*camera_gps_);
    // LIGHTS: update position

	float radius, inclination, azimuth;
	camera_controller_.update(radius, inclination, azimuth);
}

/**
* @brief Render all the renderable objects in the scene
*/
void GLSceneShadow::drawScene(const CameraFirstPerson *camera) const
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

    (current_program_iter_->second).setUniform("Ka", 0.8f, 0.1f, 0.1f);
    sphere_node_->draw(current_program_iter_->second, M, V, P);

    (current_program_iter_->second).setUniform("Ka", 0.1f, 0.1f, 0.1f);
    plane_node_->draw(current_program_iter_->second, M, V, P);
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
        renderShadow();
    }
    current_program_iter_->second.use();

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

    // LOAD MATERIAL
    loadMaterial();
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
    // LOAD MATERIAL
    loadMaterial();
    // LOAD LIGHTS
    loadLights();

    // PASS 1 (RECORD DEPTH)
    // ----------------------------------
    // Set up the viewport
    glViewport(0, 0, shadow_map_width_, shadow_map_height_);
    // Bind the frame buffer containing the shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO_);
    // Clear it
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
    // Set up the viewport
    glViewport(0, 0, width_, height_);
    // Bind to the default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Clear both the depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Activate the shadow map texture
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex_);
    current_program_iter_->second.setUniform("ShadowMap", 0);
    */
    // Select the fragment shader subroutine to shade the scene with the shadow map
    GLuint pass2Index = glGetSubroutineIndex(current_program_iter_->second.getHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
    // Disable culling (we could also switch back to back-face culling)
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // Draw the scene
    drawScene(camera_);
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
void GLSceneShadow::keyboard(unsigned char key, int x, int y)
{
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

        case 'c':
        case 'C':
            if (camera_ == fp_camera_)
            {
                camera_ = light_frustum_;
                std::cout << "Light Frustum" << std::endl;
            }
            else
            {
                camera_ = fp_camera_;
                std::cout << "First Person Camera" << std::endl;
            }
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
    delete gl_sphere_;
    delete gl_sphere_instance_;
    delete gl_plane_;
    delete gl_plane_instance_;
    delete sphere_node_;
    delete plane_node_;
    delete camera_gps_;
    delete fp_camera_;
    delete light_frustum_;
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

    delete [] buffer;
    delete [] imgBuffer;
    exit(1);
}



