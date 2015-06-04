/*
 * InputManager.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

// Local includes
#include "InputManager.hpp"
#include "Defs.hpp"         // uint32

// Global includes
#include <cstdio>   // std::printf

namespace JU
{
    /**
    * @brief Default Constructor
    *
    */
    InputManager::InputManager ()
    {
    }


    /**
    * @brief Default Destructor
    *
    */
    InputManager::~InputManager ()
    {
    }


    /**
    * @brief Initializer Function
    *
    * Initialize all components of input
    *
    * @return True if successful
    *
    */
    bool InputManager::initialize()
    {
        keyboard_.reset();

        return true;
    }


    /**
    * @brief Update event queue
    *
    * Go through all the events available since the last update
    *
    * @return True if successful
    *
    */
    bool InputManager::update()
    {
        static uint32 frame_id = 0;
        frame_id++;
        //Event handler
        SDL_Event event;

        // Send event to AntTweakBar
        //static bool handled = TwEventSDL(&event, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);

//        if (!handled)
//        {
            //Handle events on queue
            while (SDL_PollEvent( &event ) != 0)
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        //quit = true;
                        break;

                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        keyboard_(event.key);
                        break;

                    default:
                        //std::printf ("Unhandled SDL2 event %i\n", event.type);
                        break;
                }
            }
//        }

        return true;
    }


} /* namespace JU */
