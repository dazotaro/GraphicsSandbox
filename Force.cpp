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

Force::Force(ForceType type, f32 lifetime) : id_(0), type_(type), lifetime_(lifetime)
{
}



Force::~Force()
{
}



void Force::addParticle(Particle* pParticle)
{
	particle_map_[pParticle->id_] = pParticle;
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



/**
* Overloaded output operator
*
* @param out output ostream
* @param force output ostream
*
* @return output stream
*/
std::ostream& operator<<(std::ostream& out, const Force& force)
{
	out << "Force id: " 	<< force.id_ 		<< std::endl;
	out << "\tAddress: " 	<< &force 			<< std::endl;
	out << "\tType: " 		<< force.type_ 		<< std::endl;
	out << "\tLife: " 		<< force.lifetime_ 	<< std::endl;

	out << "\tLinked particles: ";
	ParticleMapConstIter particle_iter = force.particle_map_.begin();
	for (; particle_iter != force.particle_map_.end(); particle_iter++)
	{
		out << particle_iter->first << ", ";
	}
	out << std::endl;

	return out;
}

} /* namespace JU */
