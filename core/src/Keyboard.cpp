/*
 * Keyboard.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

// Local includes
#include "Keyboard.hpp"
#include "Defs.hpp"         // JU::uint32
#include "SystemLog.hpp"	// JU::SystemLog

// Global includes
#include <cstdio>           // std::printf

namespace JU
{
/**
* @brief Default Constructor
*
* Initialize all key states to UP
*
*/
Keyboard::Keyboard()
{
	reset();
}


/**
* @brief Reset Method
*
* Initialize all key states to UP
*
*/
void Keyboard::reset()
{
	// All Keys are up
	for (uint32 index = 0; index < SDL_NUM_SCANCODES; index++)
		keyState_[index] = KEY_UP;
}


/**
* @brief Handle keyboard events
*
* Process and SDL2 keyboard event
*
* @param code The SDL_KeyboardEvent of the key to check (SDL2/SDL_Scancode.h)
*
* @return True if successful
*
*/
bool Keyboard::handleEvent(SDL_KeyboardEvent& event)
{
	switch (event.state)
	{
		case SDL_PRESSED:
			keyState_[event.keysym.scancode] = KEY_DOWN;
			break;

		case SDL_RELEASED:
			keyState_[event.keysym.scancode] = KEY_UP;
			break;

		default:
			SystemLog::logMessage("Keyboard", "handleEvent(): event state not handled");
			break;

	}

	return true;
}


/**
* @brief Is Key Up?
*
* Checks the state of key
*
* @param code The SDL_Scancode of the key to check (SDL2/SDL_Scancode.h)
*
* @return True if the key is up, false otherwise
*
*/
bool Keyboard::isKeyUp(SDL_Scancode code) const
{
	if (keyState_[code] == KEY_UP)
		return true;

	return false;
}


/**
* @brief Is Key Down?
*
* Checks the state of key
*
* @param code The SDL_Scancode of the key to check (SDL2/SDL_Scancode.h)
*
* @return True if the key is down, false otherwise
*
*/
bool Keyboard::isKeyDown(SDL_Scancode code) const
{
	if (keyState_[code] == KEY_DOWN)
		return true;

	return false;
}

} /* namespace JU */
