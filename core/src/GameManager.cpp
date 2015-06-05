/*
 * GameManager.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

// Local includes
#include "GameManager.hpp"

// Global includes
#include <cstdio>       // std::printf

namespace JU
{

    GameManager::GameManager () : running_(true)
    {
        // TODO Auto-generated constructor stub

    }


    GameManager::~GameManager ()
    {
        // TODO Auto-generated destructor stub
    }


    bool GameManager::initialize()
    {
        if (!window_.initialize(800, 600))
        {
            std::printf("Window failed to initialize!!!\n");
            return false;
        }

        if (!input_manager_.initialize())
        {
            std::printf("Input Manager failed to initialize!!!\n");
            return false;
        }

        if (!state_manager_.initialize())
        {
            std::printf("Input Manager failed to initialize!!!\n");
            return false;
        }

        running_= true;

        return true;
    }


    void GameManager::loop()
    {
    	while(running_)
    	{
    		input_manager_.update();
    		running_ = input_manager_.quitting();


    	}
    }


    void GameManager::exit()
    {

    }



} /* namespace JU */
