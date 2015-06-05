/*
 * InputManager.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

#include "Keyboard.hpp" // Keyboard

namespace JU
{


    class InputManager
    {
        public:
            InputManager ();
            virtual ~InputManager ();

        public:
            bool initialize();
            bool update();
            bool quitting() const;

        public:
            Keyboard keyboard_;
            bool 	 quit_;
    };

} /* namespace JU */

#endif /* INPUTMANAGER_HPP_ */
