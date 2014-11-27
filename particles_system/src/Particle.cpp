/*
 * Particle.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

// Local includes
#include "Particle.hpp"

// Global includes
#include <iostream>		// std::cout, std::endl



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
Particle::Particle(f32 mass, const glm::vec3& position, const glm::vec3& velocity, bool is_inmortal,  JU::f32 lifetime)
			: id_(0), mass_ (mass), position_(position), velocity_(velocity), force_acc_(glm::vec3(0.0f)), is_inmortal_(is_inmortal), lifetime_(lifetime)
{
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
	force_acc_.x = 0.0f;
	force_acc_.y = 0.0f;
	force_acc_.z = 0.0f;
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
	out << "\tAddress: " 	<< &particle << std::endl;
	out << "\tMass: " 		<< particle.mass_ << std::endl;
	out << "\tLife: " 		<< particle.lifetime_ << std::endl;
	out << "\tPosition: (" 	<< particle.position_.x << ", "
							<< particle.position_.y << ", "
							<< particle.position_.z << ")" << std::endl;
	out << "\tVelocity: (" 	<< particle.velocity_.x << ", "
							<< particle.velocity_.y << ", "
							<< particle.velocity_.z << ")" << std::endl;
	out << "\tForce:    (" 	<< particle.force_acc_.x << ", "
							<< particle.force_acc_.y << ", "
							<< particle.force_acc_.z << ")" << std::endl;

	return out;
}



} /* namespace JU */
