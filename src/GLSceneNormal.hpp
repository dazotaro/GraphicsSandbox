/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENENORMAL_HPP_
#define GLSCENENORMAL_HPP_

#include "GLScene.hpp"      // GLScene
#include "Lights.hpp"       // LightPositionalVector, LightDirectionalVector, LightSpotlightVector

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
class GLSceneNormal : public GLScene
{
    public:
        // CONSTANTS
        static const int MAX_LIGHTS = 8;

        // TYPEDEFS
        typedef std::map<std::string, Node3D *> NodeMap;
        typedef NodeMap::const_iterator NodeMapIterator;

    public:
        GLSceneNormal(int width, int height);
        virtual ~GLSceneNormal();

        void init(void);
        void update(float time);
        void render(void) const;
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);
        void resize(int width, int height);

    private:
        void cleanup(void);
        void loadMaterial(void) const;
        void loadLights(void) const;

    private:
        GLMesh*          	gl_cube_;
        GLMeshInstance*  	gl_cube_instance_;
        Object3D*        	camera_gps_;
        CameraThirdPerson*  tp_camera_;
        CameraInterface*    camera_;
        GLMesh*          	gl_sphere_;
        GLMeshInstance*  	gl_sphere_instance_;
        NodeMap 			node_map_;

        LightPositionalVector  lights_positional_;
        LightDirectionalVector lights_directional_;
        LightSpotlightVector   lights_spotlight_;
};

#endif /* GLSCENENORMAL_HPP_ */
