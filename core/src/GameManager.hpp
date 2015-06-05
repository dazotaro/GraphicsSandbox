/*
 * GameManager.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include "GameStateManager.hpp" // GameStateManager
#include "Window.hpp"           // Window
#include "InputManager.hpp"		// InputManager


namespace JU
{

class GameManager
{
    public:
        GameManager ();
        virtual ~GameManager ();

        virtual bool initialize();
        virtual void loop();
        virtual void exit();

    private:
        GameStateManager state_manager_;
        Window           window_;
        InputManager     input_manager_;

        bool			 running_;
};

} /* namespace JU */

#endif /* GAMEMANAGER_HPP_ */
