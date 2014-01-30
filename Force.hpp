/*
 * Force.hpp
 *
 *  Created on: Jan 25, 2014
 *      Author: dazotaro
 */

#ifndef FORCE_HPP_
#define FORCE_HPP_

// Local includes
#include "Defs.hpp"						// custom types
#include "ParticleSystemDefs.hpp"		// ParticleList, ForceList

// Global includes
#include <iostream>			// std:;ostream, std::endl

namespace JU
{

class Force
{

	public:

		Force(ForceId id, bool transient = false, f32 lifetime = 0.0f);
		virtual ~Force();

		virtual void addParticle(Particle* particle);
		//virtual void apply(f32 time) = 0;

		void removeParticle(ParticleId particle_id);
		void releaseParticles();

		friend std::ostream& operator<<(std::ostream& out, const Force& force);

	public:

		ForceId id_;
		bool transient_;				/**< Is force transient or eternal */
		f32 lifetime_;					/**< Life left of the force (in milliseconds)*/

		ParticleMap particle_map_;	/**< Particle subject to this force */
};

} /* namespace JU */
#endif /* FORCE_HPP_ */
