/*
 * GameState.hpp
 *
 *  Created on: Jun 5, 2015
 *      Author: jusabiaga
 */

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

class GLScene;

namespace JU
{

class GameState
{
    public:
		GameState();
		virtual ~GameState();
		virtual bool load();
        virtual bool initialize();
        virtual bool update();
        virtual bool draw();
        virtual bool free();
        virtual bool unload();

        void registerGLScene(GLScene* scene);

    protected:
        GLScene* p_scene_;
};

} /* namespace JU */

#endif /* GAMESTATE_HPP_ */
