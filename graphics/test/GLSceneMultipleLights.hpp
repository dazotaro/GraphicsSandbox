/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENEMULTIPLELIGHTS_HPP_
#define GLSCENEMULTIPLELIGHTS_HPP_

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
class GLSceneMultipleLights : public GLScene
{
    public:
        static const int MAX_LIGHTS = 8;

        // TYPEDEFS
        typedef std::map<std::string, Node3D *> 		NodeMap;
        typedef std::map<std::string, GLMesh*> 			MeshMap;
        typedef std::map<std::string, GLMeshInstance*> 	MeshInstanceMap;
        typedef std::map<std::string, Object3D*>		Object3DMap;
        typedef std::map<std::string, Material*>		MaterialMap;

    public:
        GLSceneMultipleLights(int width, int height);
        virtual ~GLSceneMultipleLights();

        void init(void);
        void update(JU::f32 time);
        void render(void);
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        void resize(int width, int height);

    private:
        void cleanup(void);

        void initializePrograms();
        void initializeMaterials();
        void initializeTextures();
        void initializeObjects();
        void initializeCameras();
        void initializeLights();

        void updateCamera(JU::f32 time);
        void updateLights(JU::f32 time);

        void loadLights(void) const;

    private:
        MeshMap 			mesh_map_;
        MeshInstanceMap 	mesh_instance_map_;
        NodeMap				node_map_;
        MaterialMap			material_map_;

        CameraThirdPerson* tp_camera_;
        CameraInterface* camera_;

        bool control_camera_;

        LightPositionalVector  lights_positional_;
        LightDirectionalVector lights_directional_;
        LightSpotlightVector   lights_spotlight_;

        ArcBallController camera_controller_;
};

#endif /* GLSCENEMULTIPLELIGHTS_HPP_ */
