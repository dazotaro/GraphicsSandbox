/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENESPRITE_HPP_
#define GLSCENESPRITE_HPP_

#include "GLScene.hpp"      // GLScene

// Forward Declarations
class GLMesh;
class GLMeshInstance;
class Node3D;
class Object3D;
class CameraInterface;

/*
 * @brief Scene class
 *
 * \todo Maybe unnecessary class
 */
class GLSceneSprite : public GLScene
{
    public:
        GLSceneSprite(int width, int height);
        virtual ~GLSceneSprite();

        void init(void);
        void update(float time);
        void render(void);
        void keyboard(unsigned char key, int x, int y);
        void mouseClick(int button, int state, int x, int y);
        void resize(int width, int height);

    private:
        void clear(void);

    private:
        GLMesh*          gl_plane_;
        GLMeshInstance*  gl_plane_instance_;
        Node3D*          root_node_;
        Node3D*          child_node_;
        Object3D*        camera_gps_;
        CameraInterface* camera_;
};

#endif /* GLSCENESPRITE_HPP_ */
