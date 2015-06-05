/*
 * GameState.hpp
 *
 *  Created on: Jun 5, 2015
 *      Author: jusabiaga
 */

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

namespace JU
{

class GameState
{
    public:
		virtual ~GameState();
		virtual bool load();
        virtual bool initialize();
        virtual bool update();
        virtual bool draw();
        virtual bool free();
        virtual bool unload();
};

} /* namespace JU */

#endif /* GAMESTATE_HPP_ */
