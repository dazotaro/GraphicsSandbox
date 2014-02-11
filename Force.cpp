/*
 * Force.cpp
 *
 *  Created on: Jan 25, 2014
 *      Author: dazotaro
 */

// Local Includes
#include "Force.hpp"	// JU::Force
#include "Particle.hpp"	// JU::Particle

// Global Includes
#include <sstream>		// std::ostringstream

namespace JU
{

Force::Force(TerminationType type, f32 lifetime) : id_(0), type_(type), lifetime_(lifetime)
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


std::string Force::getInfoString() const
{
	std::ostringstream out;

	out << "Force id: " 	<< id_ 		<< std::endl;
	out << "\tAddress: " 	<< this		<< std::endl;
	out << "\tType: " 		<< type_	<< " (";

	switch (type_)
	{
	case Force::PERSISTENT:
		out << "PERSISTENT)" << std::endl;
		break;

	case Force::TRANSIENT_ON_PARTICLES:
		out << "TRANSIENT_ON_PARTICLES)" << std::endl;
		out << "\tLife: "	<< particle_map_.size()	<< "(particles)" << std::endl;

		break;

	case Force::TRANSIENT_ON_TIME:
		out << "TRANSIENT_ON_TIME)" << std::endl;
		out << "\tLife: " << lifetime_ << "(ms)" << std::endl;
		break;

	default:
		out << "UNKNOWN TYPE)";
		break;
	}


	out << "\tLinked particles: ";
	ParticleMapConstIter particle_iter = particle_map_.begin();
	for (; particle_iter != particle_map_.end(); particle_iter++)
	{
		out << particle_iter->first << ", ";
	}
	out << std::endl;

	return out.str();
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
	out << force.getInfoString() << std::endl;

	return out;
}


GravityForce::GravityForce(f32 g) : Force(PERSISTENT), g_(g)
{
}



GravityForce::~GravityForce()
{
}



void GravityForce::apply(f32 time) const
{

}



std::string GravityForce::getInfoString() const
{
	std::ostringstream out;

	out << "\tDescription: Gravity Force" << std::endl;
	out << "\tAcceleration: " << g_ << std::endl;

	return Force::getInfoString() + out.str();
}



DragForce::DragForce(f32 drag) : Force(TRANSIENT_ON_PARTICLES), drag_(drag)
{
}



DragForce::~DragForce()
{
}



void DragForce::apply(f32 time) const
{

}



std::string DragForce::getInfoString() const
{
	std::ostringstream out;

	out << "\tDescription: Drag Force" << std::endl;
	out << "\tDrag: " << drag_ << std::endl;

	return Force::getInfoString() + out.str();
}



ThrustForce::ThrustForce(const glm::vec3& force, f32 time) : Force(TRANSIENT_ON_TIME, time), force_(force)
{
}



ThrustForce::~ThrustForce()
{
}



void ThrustForce::apply(f32 time) const
{

}



std::string ThrustForce::getInfoString() const
{
	std::ostringstream out;

	out << "\tDescription: Thrust Force" << std::endl;
	out << "\tThrust Force: (" 	<< force_.x << ", "
								<< force_.y << ", "
								<< force_.z << ")" << std::endl;

	return Force::getInfoString() + out.str();
}



SpringForce::SpringForce(f32 stiffness) : Force(TRANSIENT_ON_PARTICLES), stiffness_(stiffness)
{
}



SpringForce::~SpringForce()
{
}



void SpringForce::apply(f32 time) const
{

}



std::string SpringForce::getInfoString() const
{
	std::ostringstream out;

	out << "\tDescription: Spring Force" << std::endl;
	out << "\tStiffness coefficient: " << stiffness_ << std::endl;

	return Force::getInfoString() + out.str();
}



} /* namespace JU */
