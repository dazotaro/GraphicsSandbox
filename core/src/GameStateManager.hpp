/*
 * GameStateManager.hpp
 *
 *  Created on: Jun 2, 2015
 *      Author: jusabiaga
 */

#ifndef GAMESTATEMANAGER_HPP_
#define GAMESTATEMANAGER_HPP_

// Local Includes
#include "GameState.hpp"		// GameState
// Global Includes
#include <string>				// std::string
#include <map>  				// std::map

namespace JU
{

class GameStateManager
{
    public:
        GameStateManager ();
        virtual ~GameStateManager ();

        // Interface Functions
        virtual bool initialize();
        virtual void exit();
		virtual bool changeState();
		virtual bool draw();
		void addState(const char* name, GameState* game_state);
		bool changeState(const char* name);

    private:
		// Type Definitions
		typedef std::map<std::string, GameState*> StateMap;
		typedef StateMap::iterator StateMapIter;
		// Member Variables
		StateMap 			state_map_;
		StateMapIter		curr_state_;
};

} /* namespace JU */

#endif /* GAMESTATEMANAGER_HPP_ */
