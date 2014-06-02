#include <stdio.h>                      // printf
#include "gl_core_4_2.h"                // glLoadGen generated header file
#include "GL/freeglut.h"                // Glut
#include "GLSceneSprite.hpp"            // GLSceneSprite
#include "GLSceneLighting.hpp"          // GLSceneLighting
#include "GLSceneNormal.hpp"            // GLSceneNormal
#include "GLSceneShadow.hpp"            // GLSceneShadow
#include "GLSceneParticles.hpp"			// GLSceneParticles
#include "GLSceneCometTail.hpp"         // GLSceneCometTail
#include "glm/gtc/matrix_transform.hpp" // glm::perspective
#ifndef WIN32
	#include <JU/Timer.hpp>                 // Timer
	#include <JU/FrameRateTracker.hpp>      // FrameRateTracker
#endif
#include <iostream>

#ifdef _WIN32
    #define ESC_KEY VK_ESCAPE
#elif __linux__ || __CYGWIN__
    #define ESC_KEY 27
#endif

static int winID;
static GLsizei WIDTH  = 800;
static GLsizei HEIGHT = 450;

static GLScene *scene;
#ifndef WIN32
	static FrameRateTracker frame_rate;
	static Timer timer;
#endif
//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
static void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    scene->resize(w, h);
}
 
static void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ESC_KEY:
            glutDestroyWindow(winID);
            exit(0);
            break;

        default:
            scene->keyboard(key, x, y);
            break;
    }
}
 
static void mouse(int button, int state, int x, int y)
{
    scene->mouseClick(button, state, x, y);
}

static void mouseMotion(int x, int y)
{
    scene->mouseMotion(x, y);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
static void display()
{
	#ifndef WIN32
		static float last_time = timer.getElapsedMilliseconds();
		float time = timer.getElapsedMilliseconds();
		float time_diff = time - last_time;

		scene->update(time_diff);
	#else
		scene->update(100);
	#endif

	scene->render();

    glutSwapBuffers();
    glutPostRedisplay();

    #ifndef WIN32
        // Print stats
        frame_rate.update(time);

        static int frames = 0;
        if (!(++frames % 100))
        {
            printf("\rFPS = %f; Current Shader Program = %s                        ",
                   frame_rate.getFPS(),
                   scene->getGLSLCurrentProgramString());

            fflush(stdout);
        }
        last_time = time;
    #endif
}

static void init(void)
{
    //scene = new GLSceneNormal(WIDTH, HEIGHT);
    scene = new GLSceneLighting(WIDTH, HEIGHT);
    //scene = new GLSceneShadow(WIDTH, HEIGHT);
    //scene = new GLSceneParticles(WIDTH, HEIGHT);
    //scene = new GLSceneCometTail(WIDTH, HEIGHT, 100);
    scene->init();

	#ifndef WIN32
	    timer.start();
	#endif
}

static void cleanup(void)
{
    delete scene;
}

// Main method - main entry point of application
//the freeglut library does the window creation work for us,
//regardless of the platform.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 2);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    //------------------------------------
    // glLoadGen required initialization
    int loaded = ogl_LoadFunctions();
    if(loaded == ogl_LOAD_FAILED)
    {
      //Destroy the context and abort
      return 0;
    }

    int num_failed = loaded - ogl_LOAD_SUCCEEDED; printf("Number of functions that failed to load: %i.\n",num_failed);
    //------------------------------------

    //glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100,100);
    winID = glutCreateWindow("OpenGL - First window demo");

    printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
    printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("GL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", (char *) glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Do all the initialization
    init();

    // Display function callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // handle keyboard input
    glutKeyboardFunc(keyboard);
    // Handle mouse input
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    // Start the infinite loop
    glutMainLoop();

    // Delete dynamically memory
    cleanup();
    
    return 0;
}
