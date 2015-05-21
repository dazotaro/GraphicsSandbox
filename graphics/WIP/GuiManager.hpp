/*
 * GuiManager.hpp
 *
 *  Created on: May 20, 2015
 *      Author: jusabiaga
 */

#ifndef GUIMANAGER_HPP_
#define GUIMANAGER_HPP_

// Global includes
#include <AntTweakBar.h>	// AntTweakBar
#include <map>				// std::map
#include <string>			// std::string


namespace JU
{

class GuiManager
{
	public:

	public:
		GuiManager ();
		virtual ~GuiManager ();

		void init();
		void release();

		void addBar(const char* name);
		void resize(int width, int height) const;
		void draw() const;

	public:
		bool isVisible_;
};

} /* namespace JU */

#endif /* GUIMANAGER_HPP_ */
