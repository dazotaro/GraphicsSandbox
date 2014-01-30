/*
 * ParticleSystem.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

#ifndef PARTICLESYSTEM_HPP_
#define PARTICLESYSTEM_HPP_

// Local includes
#include "Defs.hpp"						// custom types
#include "ParticleSystemDefs.hpp"		// ParticleList, ForceList

namespace JU
{

// Forward declarations
class Particle;
class Force;

class ParticleSystem
{
	public:

		ParticleSystem();
		virtual ~ParticleSystem();

		void addParticle(Particle* particle);
		void addForce(Force* force);

		void update(f32 time);

	private:

		void cleanupParticles(f32 time);
		void cleanupForces(f32 time);

		void accumulateForces(f32 time);

	private:

		// Particle Storage
		ParticleList particle_list_;
		ParticleIdList dead_particle_list_;
		uint32 num_particles_;	// we want to guarantee O(1), independent of STL implementation

		// Force Storage
		ForceList force_list_;
};

} /* namespace JU */
#endif /* PARTICLESYSTEM_HPP_ */
