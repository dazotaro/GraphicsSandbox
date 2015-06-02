/*
 * GameStateManager.hpp
 *
 *  Created on: Jun 2, 2015
 *      Author: jusabiaga
 */

#ifndef GAMESTATEMANAGER_HPP_
#define GAMESTATEMANAGER_HPP_

namespace JU
{

class GameStateManager
{
    public:
        GameStateManager ();
        virtual ~GameStateManager ();

        bool load();
        bool initialize();
        bool update();
        bool draw();
        bool free();
        bool unload();
};

} /* namespace JU */

#endif /* GAMESTATEMANAGER_HPP_ */
