/*
 * GameStateManager.hpp
 *
 *  Created on: Jun 2, 2015
 *      Author: jusabiaga
 */

#ifndef GAMESTATEMANAGER_HPP_
#define GAMESTATEMANAGER_HPP_

#include <map>  // std::map

namespace JU
{

class GameState
{
    public:
        bool load();
        bool initialize();
        bool update();
        bool draw();
        bool free();
        bool unload();
};

class GameStateManager
{
    public:
        GameStateManager ();
        virtual ~GameStateManager ();

        bool initialize();
        void exit();

    private:
        std::map<std::string, GameState> state_map_;
};

} /* namespace JU */

#endif /* GAMESTATEMANAGER_HPP_ */
