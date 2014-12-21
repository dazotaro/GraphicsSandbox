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
        typedef std::map<std::string, Node3D *> NodeMap;
        typedef NodeMap::const_iterator NodeMapIterator;

    public:
        GLSceneMultipleLights(int width, int height);
        virtual ~GLSceneMultipleLights();

        void init(void);
        void update(float time);
        void render(void);
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        void resize(int width, int height);

    private:
        void cleanup(void);
        void loadMaterial(void) const;
        void loadLights(void) const;

    private:
        GLMesh*         gl_sphere_;
        GLMeshInstance* gl_sphere_instance_;
        GLMesh*         gl_plane_;
        GLMeshInstance* gl_plane_instance_;
        Object3D*       camera_gps_;
        NodeMap			node_map_;
        //CameraFirstPerson *fp_camera_;
        CameraThirdPerson*  tp_camera_;
        CameraInterface*    camera_;
        bool control_camera_;

        LightManager  light_manager_;

        ArcBallController camera_controller_;
};

#endif /* GLSCENEMULTIPLELIGHTS_HPP_ */
