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
ParticleSystem::ParticleSystem() : num_particles_(0), particle_id_(0), force_id_(0)
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
	ForceMapIter force_iter = force_map_.begin();
	for (; force_iter != force_map_.end() ; force_iter++)
	{
		// Delete the data
		if (force_iter->second != 0)
		{
			delete force_iter->second;
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
* @return Return this particle's id
*
*/
ParticleId ParticleSystem::addParticle(Particle* pParticle)
{
	pParticle->id_ = particle_id_++;
	particle_list_.push_back(pParticle);
	num_particles_++;

	return pParticle->id_;
}



/**
* Add force to the system
*
* @param force	Pointer to the new force
*
* @return Return this force's id
*
*/
void ParticleSystem::addForce(const std::string& name, Force* pForce)
{
	pForce->id_ = force_id_++;
	force_map_[name] =  pForce;
}



/**
* Get force pointer
*
* @param name	Get pointer to this force
*
* @return Return this force pointer. Null if it does not exist
*
*/
Force* ParticleSystem::getForce(const std::string& name) const
{
	ForceMapConstIter iter = force_map_.find(name);

	if (iter != force_map_.end())
		return iter->second;

	return 0;
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
		if (!(*particle_iter)->is_inmortal_)
		{
			// Update the life left
			(*particle_iter)->lifetime_ -= time;

			// Erase it: it's dead
			if ((*particle_iter)->lifetime_ <= 0.0f)
			{
				ForceMapIter force_iter = force_map_.begin();
				for (; force_iter != force_map_.end(); force_iter++)
				{
					force_iter->second->removeParticle((*particle_iter));
				}

				// Delete the particle's data
				if (*particle_iter != 0)
				{
					delete *particle_iter;
				}

				// Remove the pointer to the particle from the list
				particle_list_.erase(particle_iter);

				num_particles_--;

				return;
			}
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
	ForceMapIter force_iter = force_map_.begin();
	for (; force_iter != force_map_.end(); force_iter++)
	{
		bool delete_force = false;

		switch ((force_iter->second)->type_)
		{
			case Force::TRANSIENT_ON_TIME:
				{
					// Update the life left
					(force_iter->second)->lifetime_ -= time;

					if ((force_iter->second)->lifetime_ <= 0.0f)
					{
						delete_force = true;
					}
				}
				break;

			case Force::TRANSIENT_ON_PARTICLES:
				{
					if ((force_iter->second)->particle_set_.size() == 0)
					{
						delete_force = true;
					}
				}
				break;

			default:
				break;
		}

		if (delete_force)
		{
			// Delete the data
			if (force_iter->second != 0)
			{
				delete force_iter->second;
			}
			// Remove the pointer from the list
			force_map_.erase(force_iter);
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
	// Reset all particle force accumulators
	// PARTICLES: for all particles
	ParticleListIter particle_iter = particle_list_.begin();
	for (; particle_iter != particle_list_.end() ; particle_iter++)
	{
		(*particle_iter)->force_acc_ = glm::vec3(0.0f);
	}

	// FORCES: for all forces
	ForceMapConstIter force_iter = force_map_.begin();
	for (; force_iter != force_map_.end(); force_iter++)
	{
		force_iter->second->apply(time);
	}
}



/**
* Numerically integrate to compute the next state for each particle
*
* @param time	Elapsed time since the last update (in milliseconds)
*
*/
void ParticleSystem::integrate(f32 time)
{
	for(ParticleListIter iter = particle_list_.begin(); iter != particle_list_.end(); ++iter)
	{
		(*iter)->position_ += time * (*iter)->velocity_;
		(*iter)->velocity_ += time * (*iter)->force_acc_ / (*iter)->mass_;
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
	// Delete expired particles
	cleanupParticles(time);
	// Delete expired forces
	cleanupForces(time);
	// Accumulate forces for all the particles involved
	accumulateForces(time);
	// Update the state via numerical integration
	integrate(time);

}



/**
* Overloaded output operator
*
* @param out output ostream
* @param particle_system particle system to output
*
* @return output stream
*/
std::ostream& operator<<(std::ostream& out, const ParticleSystem& particle_system)
{
	out << "Forces: " << particle_system.force_map_.size() << std::endl;
	out << "---------------------------------------------" << std::endl;

	ForceMapConstIter force_iter = particle_system.force_map_.begin();
	for( ; force_iter != particle_system.force_map_.end(); force_iter++)
		out << *(force_iter->second) << std::endl;

	out << "Particles: " << particle_system.num_particles_<< "(" << particle_system.particle_list_.size() << ")" << std::endl;
	out << "---------------------------------------------" << std::endl;

	ParticleListConstIter particle_iter = particle_system.particle_list_.begin();
	for( ; particle_iter != particle_system.particle_list_.end(); particle_iter++)
		out << *(*particle_iter) << std::endl;

	return out;
}




} /* namespace JU */
