/*
 * Window.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

// Local includes
#include "Defs.hpp"	// JU::uint32

// Global Include
#include <SDL2/SDL.h>		// SDL2

namespace JU
{

class Window
{
    public:
        Window ();
        virtual ~Window ();

        bool initialize(uint32 width, uint32 height);
        void exit();

    private:
    	SDL_Window* 	p_main_window_;
    	SDL_GLContext 	main_gl_context_;
    	uint32 width_;
    	uint32 height_;

};

} /* namespace JU */

#endif /* WINDOW_HPP_ */
