/*
 * Keyboard.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

// Global includes
#include <SDL2/SDL.h>    // SDL_Event


namespace JU
{
    class Keyboard
    {
        enum KeyState
        {
            KEY_DOWN = SDL_PRESSED,
            kEY_UP   = SDL_RELEASED
        };

        public:
            Keyboard();
            void reset();
            bool handleEvent(SDL_KeyboardEvent& event);
            bool isKeyUp(SDL_Scancode key) const;
            bool isKeyDown(SDL_Scancode key) const;

        private:
            KeyState keyState_[SDL_NUM_SCANCODES];
    };

} /* namespace JU */

#endif /* KEYBOARD_HPP_ */
