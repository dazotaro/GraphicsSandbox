/*
 * GameStateManager.cpp
 *
 *  Created on: Jun 2, 2015
 *      Author: jusabiaga
 */

#include "GameStateManager.hpp"

namespace JU
{

GameStateManager::GameStateManager ()
{
// TODO Auto-generated constructor stub

}


GameStateManager::~GameStateManager ()
{
// TODO Auto-generated destructor stub
}


bool GameStateManager::initialize()
{

    return true;
}


void GameStateManager::exit()
{

}


bool GameStateManager::draw()
{
	curr_state_->second->draw();

	return true;
}


void GameStateManager::addState(const char* name, GameState* game_state)
{
	if (state_map_.find(name) != state_map_.end())
	{
		SystemLog::logMessage("GameStateManager", "addState(), state already exits");
		exit();
	}
	state_map_[name] = game_state;
}


bool GameStateManager::changeState(const char* name)
{
	StateMapIter state_iter = state_map_.find(name);

	if (state_iter != state_map_.end())
	{
		SystemLog::logMessage("GameStateManager", "setCurrentState(), state already exits");

		return false;
	}

	curr_state_ = state_iter;
	curr_state_->second->initialize();

	return true;
}

} /* namespace JU */
