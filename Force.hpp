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

namespace JU
{

class Force
{

	public:

		Force(bool transient = false, f32 lifetime = 0.0f);
		virtual ~Force();

		virtual void addParticle(Particle* particle);
		//virtual void apply(f32 time) = 0;

		void removeParticle(ParticleId particle_id);
		void releaseParticles();

		ParticleMap particle_map_;	/**< Particle subject to this force */

		bool transient_;				/**< Is force transient or eternal */
		f32 lifetime_;					/**< Life left of the force (in milliseconds)*/
};

} /* namespace JU */
#endif /* FORCE_HPP_ */
