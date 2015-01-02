/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENEDEFERRED_HPP_
#define GLSCENEDEFERRED_HPP_

// Local includes
#include "GLScene.hpp"      	// GLScene
#include "Lights.hpp"       	// LightPositionalVector, LightDirectionalVector, LightSpotlightVector
#include "ArcBallController.hpp"// ArcBallController

// Forward Declarations
class GLMesh;
class GLMeshInstance;
class Node3D;
class Object3D;
class CameraInterface;
class CameraThirdPerson;
class Material;

/*
 * @brief Scene class
 *
 * \todo Maybe unnecessary class
 */
class GLSceneDeferred : public GLScene
{
    public:
        static const int MAX_POS_LIGHTS = 20;

        // TYPEDEFS
        typedef std::map<std::string, Node3D *> 		NodeMap;
        typedef std::map<std::string, GLMesh*> 			MeshMap;
        typedef std::map<std::string, GLMeshInstance*> 	MeshInstanceMap;
        typedef std::map<std::string, Object3D*>		Object3DMap;
        typedef std::map<std::string, Material*>		MaterialMap;

    public:
        GLSceneDeferred(int width, int height);
        virtual ~GLSceneDeferred();

        void init(void);
        void update(JU::f32 time);
        void render(void);
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        void resize(int width, int height);

    private:
        void reload();
        void clear(void);

        void initializePrograms();
        void initializeFBO();
        void initializeMaterials();
        void initializeTextures();
        void initializeObjects();
        void initializeCameras();
        void initializeLights();
        void initializePositionalLights();
        void initializeDirectionalLights();
        void initializeSpotlightLights();

        void updateCamera(JU::f32 time);

        void updateLights(JU::f32 time);
        void updatePositionalLights(JU::f32 time);
        void updateDirectionalLights(JU::f32 time);
        void updateSpotlightLights(JU::f32 time);

        void loadLights(void) const;
        void loadPositionalLights(void) const;
        void loadDirectionalLights(void) const;
        void loadSpotlightLights(void) const;

        void renderPass1();
        void renderPass2();

    private:
        // DEFERRED SHADING SPECIFIC
        GLuint 				deferredFBO_;
        GLuint 				depthBuf_;
        GLuint				posTex_;
        GLuint				normTex_;
        GLuint				colorTex_;
        GLuint 				pass1Index_;
        GLuint 				pass2Index_;
        bool				record_depth_;

        // ASSET MANAGEMENT
        MeshMap 			mesh_map_;
        MeshInstanceMap 	mesh_instance_map_;
        NodeMap				node_map_;
        MaterialMap			material_map_;
        NodeMap::const_iterator main_node_iter;

        // CAMERA CONTROLS
        CameraThirdPerson* tp_camera_;
        CameraInterface* camera_;
        bool control_camera_;
        ArcBallController camera_controller_;

        // LIGHT MANAGEMENT
        LightManager::LightType light_mode_;			//!< Light Mode
        JU::int32			    num_lights_;			//!< Number of lights (only one type of light active at one time)
        LightPositionalVector   lights_positional_;		//!< Vector holding all positional lights
        LightDirectionalVector  lights_directional_;	//!< Vector holding all directional lights
        LightSpotlightVector    lights_spotlight_;		//!< Vector holding all spotlights
};

#endif /* GLSCENEDEFERRED_HPP_ */
