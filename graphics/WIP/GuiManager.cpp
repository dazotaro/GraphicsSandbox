/*
 * GuiManager.cpp
 *
 *  Created on: May 20, 2015
 *      Author: jusabiaga
 */

#include "GuiManager.hpp"

namespace JU
{

GuiManager::GuiManager () : isVisible_(false)
{
}



GuiManager::~GuiManager ()
{
}



void GuiManager::init()
{
	TwInit(TW_OPENGL_CORE, NULL); // for core profile
}



void GuiManager::release()
{
	TwTerminate();
}



void GuiManager::addBar(const char* name)
{
	TwNewBar(name);
}



void GuiManager::resize(int width, int height) const
{
	TwWindowSize(width, height);
}



void GuiManager::draw() const
{
	TwDraw();  // draw the tweak bar(s)
}

} /* namespace JU */
