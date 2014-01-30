/*
 * Force.cpp
 *
 *  Created on: Jan 25, 2014
 *      Author: dazotaro
 */

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



inline void Force::addParticle(Particle* particle)
{
	particle_map_[particle->id_] = particle;
}



inline void Force::removeParticle(ParticleId particle_id)
{
	particle_map_.erase(particle_id);
}

} /* namespace JU */
