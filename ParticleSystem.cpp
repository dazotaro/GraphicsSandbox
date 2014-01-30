/*
 * ParticleSystem.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

// Local includes
#include "Force.hpp"			// JU::Force
#include "Particle.hpp"			// JU::Particle
#include "ParticleSystem.hpp"	// JU::ParticleSystem

namespace JU
{

/**
* Constructor
*/
ParticleSystem::ParticleSystem() : num_particles_(0)
{
}



/**
* Destructor
*
* \note do we need to check whether the iterator is pointing to NULL
*/
ParticleSystem::~ParticleSystem()
{
	// FORCES: for all forces
	ForceListIter force_iter = force_list_.begin();
	for (; force_iter != force_list_.end() ; force_iter++)
	{
		// Delete the data
		if (*force_iter != 0)
		{
			delete *force_iter;
		}
	}

	// PARTICLES: for all particles
	ParticleListIter particle_iter = particle_list_.begin();
	for (; particle_iter != particle_list_.end() ; particle_iter++)
	{
		// Delete the data
		if (*particle_iter != 0)
		{
			delete *particle_iter;
		}
	}
}



/**
* Add particle to the system
*
* @param particle	Pointer to the new particle
*
*/
void ParticleSystem::addParticle(Particle* particle)
{
	particle_list_.push_back(particle);
	num_particles_++;
}



/**
* Add force to the system
*
* @param force	Pointer to the new force
*
*/
void ParticleSystem::addForce(Force* force)
{
	force_list_.push_back(force);
	num_particles_++;
}



/**
* Check all particles to see if they have expired
*
* @param time	Elapsed time since the last update (in milliseconds)
*
*/
void ParticleSystem::cleanupParticles(f32 time)
{
	// PARTICLES: for all particles
	ParticleListIter particle_iter = particle_list_.begin();
	for (; particle_iter != particle_list_.end() ; particle_iter++)
	{
		// Update the life left
		(*particle_iter)->lifetime_ -= time;

		// Erase it: it's dead
		if ((*particle_iter)->lifetime_ <= 0.0f)
		{
			// Disconnect forces from this particle
			(*particle_iter)->releaseForces();

			// Delete the particle's data
			if (*particle_iter != 0)
			{
				delete *particle_iter;
			}

			// Remove the pointer to the particle from the list
			particle_list_.erase(particle_iter);

			num_particles_--;
		}
		else
		{
			// Numerically integrate this particle
		}
	}
}



/**
* Check all particles to see if they have expired
*
* @param time	Elapsed time since the last update (in milliseconds)
*
*/
void ParticleSystem::cleanupForces(f32 time)
{
	// FORCES: for all forces
	ForceListIter force_iter = force_list_.begin();
	for (; force_iter != force_list_.end(); force_iter++)
	{
		// If the force is NOT ETERNAL, check whether it should be removed
		if ((*force_iter)->transient_)
		{
			// Update the life left
			(*force_iter)->lifetime_ -= time;

			if ((*force_iter)->lifetime_ <= 0.0f)
			{
				// Delete the data
				if (*force_iter != 0)
				{
					delete *force_iter;
				}
				// Remove the pointer from the list
				force_list_.erase(force_iter);
			}
		}
	}
}



/**
* Accumulate the forces on the particles they affect
*
* @param time	Elapsed time since the last update (in milliseconds)
*
* \note do we need to check whether the iterator is pointing to NULL
*/
void ParticleSystem::accumulateForces(f32 time)
{
	// FORCES: for all forces
	ForceListIter force_iter = force_list_.begin();
	for (; force_iter != force_list_.end(); force_iter++)
	{
		// If the force is NOT ETERNAL, check whether it should be removed
		if ((*force_iter)->transient_)
		{
			// Update the life left
			(*force_iter)->lifetime_ -= time;

			if ((*force_iter)->lifetime_ <= 0.0f)
			{
				// Delete the data
				if (*force_iter != 0)
				{
					delete *force_iter;
				}
				// Remove the pointer from the list
				force_list_.erase(force_iter);
			}
		}
	}
}



/**
* Update all particles
*
* @param time	Elapsed time since the last update (in milliseconds)
*
* \note do we need to check whether the iterator is pointing to NULL
*/
void ParticleSystem::update(f32 time)
{
	// Accumulate forces for all the particles involved
	accumulateForces(time);


}

} /* namespace JU */
