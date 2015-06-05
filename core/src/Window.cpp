/*
 * Window.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

// Local Include
#include "Window.hpp"


namespace JU
{

Window::Window() : p_main_window_(nullptr), main_gl_context_(nullptr), width_(640), height_(480)
{
}


Window::~Window()
{
    // TODO Auto-generated destructor stub
}


/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}



void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}


bool Window::initialize(uint32 width, uint32 height)
{
	width_  = width;
	height_ = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */

    /* Request opengl 4.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //------------------------------------
    // glLoadGen required initialization
    int loaded = ogl_LoadFunctions();
    if(loaded == ogl_LOAD_FAILED)
    {
        //Destroy the context and abort
        return 0;
    }

    int num_failed = loaded - ogl_LOAD_SUCCEEDED;
    printf("Number of functions that failed to load: %i.\n",num_failed);
    //------------------------------------


    /* Create our window centered at 512x512 resolution */
    p_main_window_ = SDL_CreateWindow("Testing SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width_, height_, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!p_main_window_) /* Die if creation failed */
        sdldie("Unable to create window");

    checkSDLError(__LINE__);

    /* Create our opengl context and attach it to our window */
    main_gl_context_ = SDL_GL_CreateContext(p_main_window_);
    checkSDLError(__LINE__);


    /* This makes our buffer swap
     * 1 = syncronized with the monitor's vertical refresh
     * 0 = immediate */
    SDL_GL_SetSwapInterval(0);

    printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
    printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
    printf("GL version: %s\n", glGetString(GL_VERSION));
    printf("GLSL version: %s\n", (char *) glGetString(GL_SHADING_LANGUAGE_VERSION));

    // How many active textures do we have access to?
    GLint data;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &data);
    printf("Max Number of Combined Texture Units: %i\n", data);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &data);
    printf("Max Number of Fragment Texture Units: %i\n", data);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &data);
    printf("Max Number of Vertex Texture Units: %i\n", data);
    glGetIntegerv(GL_MAX_FRAGMENT_IMAGE_UNIFORMS, &data);
    printf("Max Number of Fragment Image Variables: %i\n", data);


    return true;
}


void Window::exit()
{
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(main_gl_context_);
    SDL_DestroyWindow(p_main_window_);
    SDL_Quit();
}

} /* namespace JU */
