/*
 * Particle.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

// Local includes
#include "Particle.hpp"
#include "Force.hpp"

namespace JU
{

/**
* Constructor
*
* @param position	Initial position
* @param velocity	Initial velocity
* @param mass		Mass of the particle
* @param lifetime	Life (in seconds) of this particle
*/
Particle::Particle(ParticleId id, f32 mass, f32 position[3], f32 velocity[3], f32 lifetime)
					: id_(id), mass_ (mass), lifetime_(lifetime)
{
	for (uint8 i = 0; i < 3; i++)
	{
		position_[i] = position[i];
		velocity_[i] = velocity[i];
	}
}



/**
* Destructor
*/
Particle::~Particle()
{
	// TODO Auto-generated destructor stub
}



/**
* Reset force accumulator
*
*/
inline void Particle::resetForceAcc()
{
	force_acc_[0] = 0.0f;
	force_acc_[1] = 0.0f;
	force_acc_[2] = 0.0f;
}



/**
* Add force acting on this particle
*
* #param pForce Pointer to new acting force
*
*/
inline void Particle::addForce(Force* pForce)
{
	force_set_.insert(pForce);
}



/**
* Remove force acting on this particle
*
* #param pForce Pointer to the force to be deleted
*
*/
void Particle::removeForce(Force* pForce)
{
	force_set_.erase(pForce);
}



/**
* Notify all forces acting on this particle that it is not longer under their influence
*
*/
void Particle::releaseForces()
{
	for (ForceSetIter iter = force_set_.begin(); iter != force_set_.end(); iter++)
	{
		(*iter)->removeParticle(id_);
	}
}



/**
* Overloaded output operator
*
* @param out output ostream
* @param particle output ostream
*
* @return output stream
*/
std::ostream& operator<<(std::ostream& out, const Particle& particle)
{

	out << "Particle id: " 	<< particle.id_ << std::endl;
	out << "\tMass: " 		<< particle.mass_ << std::endl;
	out << "\tLife: " 		<< particle.lifetime_ << std::endl;
	out << "\tPosition: (" 	<< particle.position_[0] << ", "
							<< particle.position_[1] << ","
							<< particle.position_[2] << ")" << std::endl;
	out << "\tVelocity: (" 	<< particle.velocity_[0] << ", "
							<< particle.velocity_[1] << ","
							<< particle.velocity_[2] << ")" << std::endl;
	out << "\tForce:    (" 	<< particle.force_acc_[0] << ", "
							<< particle.force_acc_[1] << ","
							<< particle.force_acc_[2] << ")" << std::endl;

	out << "\tLinked forces: ";
	ForceSetIter force_iter = particle.force_set_.begin();
	for (; force_iter != particle.force_set_.end(); force_iter++)
	{
		out << (*force_iter)->id_ << ", ";
	}
	out << std::endl;

	return out;
}



} /* namespace JU */
