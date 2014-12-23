#include <stdio.h>              // printf
#include <GL/glew.h>
#include "GL/freeglut.h"        // Glut
#include "Mesh.hpp"             // Mesh
#include "GLMesh.hpp"           // GLMesh
#include "GLMeshInstance.hpp"   // GLMeshInstance
#include "ShapeHelper.hpp"      // buildPlane()
#include "Node3D.hpp"           // Node3D
#include "CameraFirstPerson.hpp"// CameraFirstPerson
#include "CameraThirdPerson.hpp"// CameraThirdPerson
#include "GLSLProgram.hpp"      // GLSLProgram
#include "glm/gtc/matrix_transform.hpp" // glm::perspective

#ifdef _WIN32
    #define ESC_KEY VK_ESCAPE
#elif __linux__ || __CYGWIN__
    #define ESC_KEY 27
#endif

static int winID;
static GLsizei WIDTH  = 800;
static GLsizei HEIGHT = 450;

static GLMesh*              gl_cube;
static GLMeshInstance*      gl_cube_instance;
static Node3D*              cube_node;
static Object3D*            camera_gps;
static CameraFirstPerson*   camera;
static GLSLProgram          glsl_program;

const std::string strVertexShader(
        "#version 410\n"
        "layout(location = 0) in vec3 VertexPosition;\n"
        "layout(location = 1) in vec4 VertexColor;\n"
        "layout(location = 2) in vec3 VertexNormal;\n"
        "layout(location = 3) in vec2 VertexTexture;\n"
        "out vec4 Color;\n"
        "void main()\n"
        "{\n"
        "   Color = VertexColor;\n"
        "   gl_Position = vec4(VertexPosition, 1.0) - vec4(0.0, 0.0, 0.0, 0.0);\n"
        "}\n"
);

const std::string strFragmentShader(
        "#version 410\n"
        "in vec4 Color;\n"
        "out vec4 outputColor;\n"
        "void main()\n"
        "{\n"
        "   outputColor = Color;\n"
        "}\n"
);

void initShaderProgram(void)
{
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}
 
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
            cube_node->rotateX(-1.0f);
            break;

        case 'd':
        case 'D':
            cube_node->rotateX(1.0f);
            break;

        case 'q':
        case 'Q':
            cube_node->rotateY(1.0f);
            break;

        case 'e':
        case 'E':
            cube_node->rotateY(-1.0f);
            break;

        case 'w':
        case 'W':
            cube_node->rotateZ(-1.0f);
            break;

        case 's':
        case 'S':
            cube_node->rotateZ(1.0f);
            break;

        case 'k':
        case 'K':
            camera_gps->rotateY(1.0f);  // Yaw
            break;

        case ';':
        case ':':
            camera_gps->rotateY(-1.0f);  // Yaw
            break;

        case 'i':
        case 'I':
            camera_gps->rotateZ(1.0f);  // Roll
            break;

        case 'p':
        case 'P':
            camera_gps->rotateZ(-1.0f); // Roll
            break;

        case 'o':
        case 'O':
            camera_gps->rotateX(-1.0f);  // Pitch
            break;

        case 'l':
        case 'L':
            camera_gps->rotateX(1.0f); // Pitch
            break;

        case 'u':
        case 'U':
            camera_gps->translate(glm::vec3(0.0f, 0.0f, -0.5f));
            break;

        case 'j':
        case 'J':
            camera_gps->translate(glm::vec3(0.0f, 0.0f, 0.5f));
            break;

        case ESC_KEY:
            glutDestroyWindow(winID);
            exit(0);
            break;
    }
}
 
//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Model Matrix
    glm::mat4 M(1.0f);
    // View matrix
    glm::mat4 V(camera->getViewMatrix());
    // Perspective Matrix
    glm::mat4 P(camera->getPerspectiveMatrix());
    // Draw each object
    cube_node->draw(glsl_program, M, V, P);

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void)
{

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    // SHADERS
    //--------
    if (!glsl_program.compileShaderFromFile("shaders/diffuse_simple.vert",GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s", glsl_program.log().c_str());
        exit(1);
    }
    if(!glsl_program.compileShaderFromFile("shaders/diffuse_simple.frag",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s", glsl_program.log().c_str());
        exit(1);
    }
    if(!glsl_program.link())
    {
        printf("Shader program failed to link!\n%s", glsl_program.log().c_str());
        exit(1);
    }

    glsl_program.use();

    // Create Cube Mesh
    gl_cube = new GLMesh(Graphics::buildMesh(Graphics::SPHERE));
    gl_cube->initVBOs();
    // Create instance of GLMEsh (there could be more than one)
    gl_cube_instance = new GLMeshInstance(gl_cube, 1.0f, 1.0f, 1.0f);

    // Give the cube a position and orientation
    Object3D cube_3d(glm::vec3(0.0f, 0.0f, -5.0f), // Model's position
                     glm::vec3(1.0f, 0.0f,  0.0f), // Model's X axis
                     glm::vec3(0.0f, 1.0f,  0.0f), // Model's Y axis
                     glm::vec3(0.0f, 0.0f,  1.0f));// Model's Z axis
    NodePointerList children;
    //Node3D cube_node(cube_3d, dynamic_cast<DrawInterface*>(gl_cube_instance), children, true);
    cube_node = new Node3D(cube_3d, gl_cube_instance, children, true);

    // Create the Camera
    camera_gps = new Object3D(glm::vec3(0.0f, 0.0f, 0.0f), // Camera's position (eye's coordinates)
                              glm::vec3(1.0f, 0.0f, 0.0f), // Camera's X axis
                              glm::vec3(0.0f, 1.0f, 0.0f), // Camera's Y axis
                              glm::vec3(0.0f, 0.0f, 1.0f));// VIEWING AXIS (the camera is looking into its NEGATIVE Z axis)
    camera = new CameraFirstPerson(CameraIntrinsic(90.f, WIDTH/(float)HEIGHT, 1.f, 1000.f), *camera_gps);
    //camera = new CameraThirdPerson(CameraIntrinsic(90.f, WIDTH/(float)HEIGHT, 1.f, 1000.f), dynamic_cast<Object3D *>(cube_node));
}

void clear(void)
{
    delete gl_cube_instance;
    delete gl_cube;
    delete cube_node;
    delete camera_gps;
    delete camera;
}

// Main method - main entry point of application
//the freeglut library does the window creation work for us,
//regardless of the platform.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100,100);
    winID = glutCreateWindow("OpenGL - First window demo");

    printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
    printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("GL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", (char *) glGetString(GL_SHADING_LANGUAGE_VERSION));

//#if defined(USING_GLEW)
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
    // Problem: glewInit failed, something is seriously wrong.
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
//#endif
    
    // Do all the initialization
    init();

    // Display function callback
    glutDisplayFunc(display);
    // Function to handle resizing the window
    glutReshapeFunc(reshape);
    // handle keyboard input
    glutKeyboardFunc(keyboard);
    // Start the infinite loop
    glutMainLoop();

    // Delete dynamically memory
    clear();
    
    return 0;
}
