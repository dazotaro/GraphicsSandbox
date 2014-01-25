/*
 * ParticleSystemDefs.hpp
 *
 *  Created on: Jan 25, 2014
 *      Author: aitatxo
 */

#ifndef PARTICLESYSTEMDEFS_HPP_
#define PARTICLESYSTEMDEFS_HPP_

// Global includes
#include <list>		// std::list


namespace JU
{
	// Forward Declarations
	class Particle;
	class Force;

	// Typedefs
	typedef std::list<Particle*> ParticleList;
	typedef ParticleList::const_iterator ParticleListConstIter;
	typedef ParticleList::iterator ParticleListIter;

	typedef std::list<Force*> ForceList;
	typedef ForceList::const_iterator ForceListConstIter;
	typedef ForceList::iterator ForceListIter;
} /* namespace JU */


#endif /* PARTICLESYSTEMDEFS_HPP_ */
