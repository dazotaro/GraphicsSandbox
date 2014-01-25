/*
 * ParticleSystem.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

// Local includes
#include "ParticleSystem.hpp"	// ParticleSystem
#include "Particle.hpp"			// Particle

namespace JU
{

/**
* Constructor
*/
ParticleSystem::ParticleSystem ()
{
}



/**
* Destructor
*/
ParticleSystem::~ParticleSystem ()
{
}



/**
* Add particle to the system
*
* @param particle	New particle
*
*/
void ParticleSystem::addParticle (Particle* particle)
{
	particle_list_.push_back (particle);
}



/**
* Update all particles
*
* @param time	Elapsed time since the last update (in milliseconds)
*
*/
void ParticleSystem::update (f32 time)
{
	ParticleListIter particle_iter = particle_list_.begin();
	for (; particle_iter != particle_list_.end() ; particle_iter++)
	{
		// Update the life left
		(*particle_iter)->lifetime_ -= time;

		if ((*particle_iter)->lifetime_ <= 0.0f)
		{
			particle_list_.erase(particle_iter);
		}
		else
		{
			// Numerically integrate this particle
		}
	}
}

} /* namespace JU */
