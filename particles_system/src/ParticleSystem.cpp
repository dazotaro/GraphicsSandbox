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
ParticleSystem::ParticleSystem(const Object3D &object3d)
						: Object3D(object3d), num_particles_(0), particle_id_(0), force_id_(0)
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
* Add collision plane
*
* @param plane	Plane to test collisions with
*
*/
void ParticleSystem::addCollisionPlane(const std::string name, const Plane& plane)
{
	collion_plane_map_[name] = plane;
}



/**
* Get vector of positions
*
* @param vPositions	Get all the positions
*
*/
void ParticleSystem::getPositions(std::vector<glm::vec3>& vPositions) const
{
	JU::uint32 size = particle_list_.size();
	vPositions.resize(size);
	JU::uint32 index = 0;

	for (ParticleListConstIter iter = particle_list_.begin(); iter != particle_list_.end(); ++iter)
	{
		vPositions[index++] = (*iter)->position_;
	}
}



/**
* Get vector of colors
*
* @param vColors	Vector with color for each particle
*
*/
void ParticleSystem::getColors(std::vector<glm::vec4>& vColors) const
{
	JU::uint32 size = particle_list_.size();
	vColors.resize(size);
	JU::uint32 index = 0;

	for (ParticleListConstIter iter = particle_list_.begin(); iter != particle_list_.end(); ++iter)
	{
		vColors[index++] = (*iter)->color_;
	}
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
	glm::mat4 model (getTransformToParent());

	ForceMapConstIter force_iter = force_map_.begin();
	for (; force_iter != force_map_.end(); force_iter++)
	{
		force_iter->second->apply(model, time);
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
	// DEBUG start
	/*
	JU::ParticleListConstIter iter = particle_list_.begin();
	glm::vec3 vel = (*iter)->velocity_;
	glm::vec3 acc = (*iter)->force_acc_ / (*iter)->mass_;
	std::printf ("Time     	   = %f\n", time);
	std::printf ("Velocity     = (%f, %f, %f)\n", vel.x, vel.y, vel.z);
	std::printf ("Acceleration = (%f, %f, %f)\n", acc.x, acc.y, acc.z);
	*/
	// DEBUG end

	for(ParticleListIter iter = particle_list_.begin(); iter != particle_list_.end(); ++iter)
	{
		if ((*iter)->update_)
		{
			(*iter)->position_ += time * (*iter)->velocity_ * 0.001f;
			(*iter)->velocity_ += time * (*iter)->force_acc_ / (*iter)->mass_  * 0.001f;
		}
	}
}



/**
* Handle possible collisions
*
*/
void ParticleSystem::handleCollisions()
{
	static const JU::f32 EPSILON = 0.001f;

	for(ParticleListIter particle_iter = particle_list_.begin(); particle_iter != particle_list_.end(); ++particle_iter)
	{
		if ((*particle_iter)->update_)
		{
			for (PlaneMapConstIter plane_iter = collion_plane_map_.begin(); plane_iter != collion_plane_map_.end(); ++plane_iter)
			{
				// If the particle is on the back side of the plane
				glm::vec3 plane2particle ((*particle_iter)->position_ - plane_iter->second.point_);
				if (glm::dot(plane2particle, plane_iter->second.normal_) < EPSILON)
				{
					// and if it is moving away from the front side
					JU::f32 vnorm_proj (glm::dot((*particle_iter)->velocity_, plane_iter->second.normal_));
					if (vnorm_proj < 0.0f)
					{
						glm::vec3 vnorm  (vnorm_proj * plane_iter->second.normal_);
						glm::vec3 vparal ((*particle_iter)->velocity_ - vnorm);

						// To compute the new velocity we keep the component parallel to the plane and we reverse the normal one
						(*particle_iter)->velocity_ = vparal - vnorm;
					}
				}
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
	// Delete expired particles
	cleanupParticles(time);
	// Handle possible collisions (only for non-excluded particles and if collision planes are available)
	handleCollisions();
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
