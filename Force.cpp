/*
 * Force.cpp
 *
 *  Created on: Jan 25, 2014
 *      Author: dazotaro
 */

// Local Includes
#include "Force.hpp"	// JU::Force
#include "Particle.hpp"	// JU::Particle

namespace JU
{

Force::Force(bool transient, f32 lifetime) : transient_(transient), lifetime_(lifetime)
{
}



Force::~Force()
{
}



void Force::addParticle(Particle* particle)
{
	particle_map_[particle->id_] = particle;
}



void Force::removeParticle(ParticleId particle_id)
{
	particle_map_.erase(particle_id);
}



void Force::releaseParticles()
{
	ParticleMapIter particle_iter = particle_map_.begin();
	for ( ; particle_iter != particle_map_.end(); particle_iter++)
	{
		(particle_iter->second)->removeForce(this);
	}
}

} /* namespace JU */
