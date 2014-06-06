/*
 * main.cpp
 *
 *  Created on: May 2, 2013
 *      Author: jusabiaga
 */

#include "Mesh2.hpp"         // Vertex, Face, Mesh
#include "ShapeHelper2.hpp"  // buildPlane()
#include "Object3D.hpp"     // Object3D
#include <iostream>         // std::cout, std::endl
#include "DebugGlm.hpp"     // overloaded ostream operator for glm types
#include "GLMesh.hpp"
#include "GLMeshInstance.hpp"
#include "CameraFirstPerson.hpp"    // testCamera
void testPlane(void)
{
    // Create Mesh
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::PLANE);

    mesh.export2OBJ("plane.obj");
}

void testCube(void)
{
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CUBE);

    mesh.export2OBJ("cube.obj");
}

void testCylinder(void)
{
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CYLINDER, 8);

    mesh.export2OBJ("cylinder_8.obj");
}

void testCone(void)
{
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::CONE, 8);

    mesh.export2OBJ("cone_8.obj");
}

void testSphere(void)
{
    Mesh2 mesh;
    ShapeHelper2::buildMesh(mesh, ShapeHelper2::SPHERE, 16, 8);

    mesh.export2OBJ("sphere_16_8.obj");
}

/*
 * void testGLMeshInstance(void)
{
    // Create Mesh
    GLMesh *gl_mesh01 = new GLMesh(buildMesh(CUBE));
    GLMesh *gl_mesh02 = new GLMesh(buildMesh(CUBE));

    GLMeshInstance *gl_mesh_instance_a = new GLMeshInstance(gl_mesh01);
    GLMeshInstance *gl_mesh_instance_b = new GLMeshInstance(gl_mesh01);
    GLMeshInstance *gl_mesh_instance_c = new GLMeshInstance(gl_mesh02);

    delete gl_mesh_instance_a;
    delete gl_mesh_instance_b;
    delete gl_mesh_instance_c;

    delete gl_mesh02;
    delete gl_mesh01;
}


void testGLM(void)
{
    glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 model(1.0f, 0.0f,  0.0f, 0.0f,
                    0.0f, 1.0f,  0.0f, 0.0f,
                    0.0f, 0.0f,  1.0f, 0.0f,
                    0.0f, 0.0f, -5.0f, 1.0f);

    std::cout << vec << std::endl;
    std::cout << model << std::endl;
    std::cout << model * vec << std::endl;
}


void testCamera(void)
{
    Object3D camera_object_3d(glm::vec3(0.0f, 0.0f, 0.0f), // position
                              glm::vec3(0.0f, 1.0f, 0.0f),  // longitudinal axis
                              glm::vec3(0.0f, 0.0f, 1.0f),  // vertical axis
                              glm::vec3(1.0f, 0.0f, 0.0f)); // lateral axis
    CameraFirstPerson camera(CameraIntrinsic(90.f, 640/(float)480, 1.f, 1000.f),
                             camera_object_3d);

    CameraIntrinsic intrinsic(90.f, 640/(float)480, 1.f, 1000.f);

    std::cout << "Perspective Matrix =" << std::endl;
    std::cout << intrinsic.getPerspectiveMatrix() << std::endl;

    std::cout << "View Matrix =" << std::endl;
    std::cout << camera.getViewMatrix() << std::endl;
}

void testObject3D(void)
{
    Object3D camera_object_3d(glm::vec3(11.0f, 22.0f, 33.0f), // position
                              glm::vec3(0.0f, 1.0f, 0.0f),  // longitudinal axis
                              glm::vec3(0.0f, 0.0f, 1.0f),  // vertical axis
                              glm::vec3(1.0f, 0.0f, 0.0f)); // lateral axis

    std::cout << "To Parent Transform =" << std::endl;
    std::cout << camera_object_3d.getTransformToParent() << std::endl;

    std::cout << "From Parent Transform =" << std::endl;
    std::cout << camera_object_3d.getTransformFromParent() << std::endl;

    std::cout << "Product = " << std::endl;
    std::cout << camera_object_3d.getTransformFromParent() * camera_object_3d.getTransformToParent() << std::endl;
}
*/

int main(void)
{
    // Test
    testPlane();
    testCube();
    testCylinder();
    testCone();
    testSphere();
    //testObject3D();
    //testGLMeshInstance();
    //testGLM();
    //testCamera();
    //testObject3D();

    return 0;
}
