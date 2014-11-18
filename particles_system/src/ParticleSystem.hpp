/*
 * ParticleSystem.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

#ifndef PARTICLESYSTEM_HPP_
#define PARTICLESYSTEM_HPP_

// Local includes
#include <JU/core/Defs.hpp>				// custom types
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

		ParticleId addParticle(Particle* pParticle);

		void addForce(const std::string& name, Force* pForce);
		Force* getForce(const std::string& name) const;

		void update(f32 time);

	private:

		void cleanupParticles(f32 time);
		void cleanupForces(f32 time);

		void accumulateForces(f32 time);

	public:

		// Friend functions
		friend std::ostream& operator<<(std::ostream& out, const ParticleSystem& particle_system);

	private:

		// Particle Storage
		ParticleList 	particle_list_;
		uint32	 		num_particles_;	// we want to guarantee O(1), independent of STL implementation
		ParticleId 		particle_id_;

		// Force Storage
		ForceMap 		force_map_;
		ForceId 		force_id_;
};

} /* namespace JU */
#endif /* PARTICLESYSTEM_HPP_ */
