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
#include <map>		// std::map
#include <set>		// std::unordered_set
#include <vector>	// std::vector
#include <string>	// std::string

namespace JU
{
	// Forward Declarations
	class Particle;
	class Force;

	// Typedefs
	typedef uint32 ParticleId;
	typedef uint32 ForceId;

	typedef std::list<Particle*> 			ParticleList;
	typedef ParticleList::const_iterator 	ParticleListConstIter;
	typedef ParticleList::iterator 			ParticleListIter;

	typedef std::map<ParticleId, Particle*> ParticleMap;
	typedef ParticleMap::const_iterator 	ParticleMapConstIter;
	typedef ParticleMap::iterator 			ParticleMapIter;

	typedef std::set<Particle*> 			ParticleSet;
	typedef ParticleSet::const_iterator 	ParticleSetConstIter;
	typedef ParticleSet::iterator 			ParticleSetIter;

	typedef std::list<Force*> 				ForceList;
	typedef ForceList::const_iterator 		ForceListConstIter;
	typedef ForceList::iterator 			ForceListIter;

	typedef std::map<std::string, Force*> 	ForceMap;
	typedef ForceMap::const_iterator 		ForceMapConstIter;
	typedef ForceMap::iterator 				ForceMapIter;

	typedef std::set<Force*> 				ForceSet;
	typedef ForceSet::iterator 				ForceSetIter;

	typedef std::vector<ForceId>			ForceIdVector;
	typedef ForceIdVector::const_iterator	ForceIdVectorConstIter;
	typedef ForceIdVector::iterator			ForceIdVectorIter;
} /* namespace JU */


#endif /* PARTICLESYSTEMDEFS_HPP_ */
