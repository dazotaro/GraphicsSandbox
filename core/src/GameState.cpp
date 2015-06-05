/*
 * GameState.cpp
 *
 *  Created on: Jun 5, 2015
 *      Author: jusabiaga
 */

// Local includes
#include "GameState.hpp"

// Global includes
#include <JU/graphics/GLScene.hpp>

namespace JU
{

GameState::GameState() : p_scene_(nullptr)
{
}


GameState::~GameState()
{
}


bool GameState::load()
{

    return true;
}


bool GameState::initialize()
{

    return true;
}



bool GameState::update()
{

    return true;
}



bool GameState::draw()
{
	if (p_scene_)
		p_scene_->render();

    return true;
}



bool GameState::free()
{

    return true;
}



bool GameState::unload()
{

    return true;
}


void GameState::registerGLScene(GLScene* scene)
{
	p_scene_ = scene;
}



} /* namespace JU */
