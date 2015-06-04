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

namespace JU
{

class GameManager
{
    public:
        GameManager ();
        virtual ~GameManager ();

        bool initialize();
        void loop();
        void exit();

    private:
        GameStateManager state_manager_;
        Window           window_;
        InputManager     input_manager_;

        bool			 running_;
};

} /* namespace JU */

#endif /* GAMEMANAGER_HPP_ */
