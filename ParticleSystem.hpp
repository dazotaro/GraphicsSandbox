/*
 * ParticleSystem.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

#ifndef PARTICLESYSTEM_HPP_
#define PARTICLESYSTEM_HPP_

// Local includes
#include "Defs.hpp"	// custom types

// Global includes
#include <list>		// std::list

namespace JU
{

// Forward declarations
class Particle;

class ParticleSystem
{
	public:

		ParticleSystem ();
		virtual ~ParticleSystem ();

		void addParticle (Particle* particle);
		void update (f32 time);

	private:

		typedef std::list<Particle*> ParticleList;
		typedef ParticleList::const_iterator ParticleListConstIter;
		typedef ParticleList::iterator ParticleListIter;

		ParticleList particle_list_;
};

} /* namespace JU */
#endif /* PARTICLESYSTEM_HPP_ */
