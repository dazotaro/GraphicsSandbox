/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENESHADOW_HPP_
#define GLSCENESHADOW_HPP_

#include "GLScene.hpp"      // GLScene
#include "Lights.hpp"       		// LightPositionalVector, LightDirectionalVector, LightSpotlightVector
#include "ArcBallController.hpp"	// ArcBallController

// Forward Declarations
class GLMesh;
class GLMeshInstance;
class Node3D;
class Object3D;
class CameraInterface;
class CameraFirstPerson;
class CameraThirdPerson;

/*
 * @brief Scene class
 *
 * \todo Maybe unnecessary class
 */
class GLSceneShadow : public GLScene
{
    public:
        static const int MAX_LIGHTS = 8;

        // TYPEDEFS
        typedef std::map<std::string, GLMesh *> MeshMap;
        typedef MeshMap::const_iterator MeshMapIterator;
        typedef std::map<std::string, GLMeshInstance *> MeshInstanceMap;
        typedef MeshInstanceMap::const_iterator MeshInstanceMapIterator;
        typedef std::map<std::string, Node3D *> NodeMap;
        typedef NodeMap::const_iterator NodeMapIterator;

    public:
        GLSceneShadow(int width, int height);
        virtual ~GLSceneShadow();

        void init(void);
        void setupFBO(void);
        void update(float time);
        void render(void);
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        void resize(int width, int height);

    private:

        // Initialization functions
    	void initializeGLSLPrograms();
    	void initializeMeshes();
    	void initializeCameras();
    	void initializeLights();
    	void initializeObjects();
    	void initializeShadowMap();

        void renderPerfragmentLighting(void) const;
        void renderShadow(void) const;
        void drawScene(const CameraInterface* camera) const;
        void clear(void);
        void loadLights(void) const;
        void spitOutDepthBuffer() const;

    private:
        GLMeshInstance*  	gl_plane_shadow_instance_;
        Node3D*          	shadow_plane_node_;
        Object3D*        	camera_gps_;
        CameraFirstPerson*	fp_camera_;
        CameraFirstPerson*	light_frustum_;
        CameraThirdPerson*	tp_camera_;
        CameraInterface* 	camera_;
        MeshMap 			mesh_map_;
        MeshInstanceMap 	mesh_instance_map_;
        NodeMap				node_map_;

        int shadow_map_width_;
        int shadow_map_height_;
        bool pcf_enabled_;
        GLuint shadowFBO_;
        GLuint depthTex_;

        bool control_camera_;

        LightPositionalVector  lights_positional_;
        LightDirectionalVector lights_directional_;
        LightSpotlightVector   lights_spotlight_;

        ArcBallController camera_controller_;
};

#endif /* GLSCENESHADOW_HPP_ */
