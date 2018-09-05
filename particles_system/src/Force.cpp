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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

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
	particle_set_.insert(pParticle);
}



void Force::removeParticle(Particle* pParticle)
{
	particle_set_.erase(pParticle);
}



std::string Force::getInfoString() const
{
	std::ostringstream out;

	out << "Force id: " 	<< id_ 		<< std::endl;
	out << "\tAddress: " 	<< this		<< std::endl;

	switch (type_)
	{
		case Force::PERSISTENT:
			out << "\tType: " << type_	<< " (PERSISTENT)" << std::endl;
			break;

		case Force::TRANSIENT_ON_PARTICLES:
			out << "\tType: " << type_	<< " (TRANSIENT_ON_PARTICLES)" << std::endl;
			out << "\tLife: " << particle_set_.size()	<< " (particles)" << std::endl;

			break;

		case Force::TRANSIENT_ON_TIME:
			out << "\tType: " << type_	<< " (TRANSIENT_ON_TIME)" << std::endl;
			out << "\tLife: " << lifetime_ << " (milliseconds)" << std::endl;
			break;

		default:
			out << "UNKNOWN TYPE)";
			break;
	}


	out << "\tLinked particles: ";
	ParticleSetConstIter particle_iter = particle_set_.begin();
	for (; particle_iter != particle_set_.end(); particle_iter++)
	{
		out << *particle_iter << ", ";
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



void GravityForce::apply(const glm::mat4& model, f32 time) const
{
	ParticleSetIter particle_iter = particle_set_.begin();
	for (; particle_iter != particle_set_.end(); particle_iter++)
	{
		(*particle_iter)->force_acc_ += glm::vec3(0.0f, -g_, 0.0f) * (*particle_iter)->mass_;
	}
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



void DragForce::apply(const glm::mat4& model, f32 time) const
{
	ParticleSetConstIter particle_iter = particle_set_.begin();
	for (; particle_iter != particle_set_.end(); particle_iter++)
	{
		(*particle_iter)->force_acc_ += -drag_ * (*particle_iter)->velocity_;
	}
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



void ThrustForce::apply(const glm::mat4& model, f32 time) const
{
	ParticleSetConstIter particle_iter = particle_set_.begin();
	for (; particle_iter != particle_set_.end(); particle_iter++)
	{
		(*particle_iter)->force_acc_ += force_;
	}
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



SpringForce::SpringForce(f32 ks, f32 equilibrium_distance)
				: Force(TRANSIENT_ON_PARTICLES), ks_(ks), equilibrium_distance_(equilibrium_distance)
{
}



SpringForce::~SpringForce()
{
}



void SpringForce::apply(const glm::mat4& model, f32 time) const
{
	if (particle_set_.size() != 2)
		std::printf("Spring forces with more than two particles not yet supported\n");

	ParticleSetConstIter iter = particle_set_.begin();
	ParticleSetConstIter particle1 = iter++;
	ParticleSetConstIter particle2 = iter;

	glm::vec3 P1toP2 ((*particle2)->position_ - (*particle1)->position_);
	JU::f32 P1toP2distance = glm::l2Norm(P1toP2);

	glm::vec3 P1toP2normalized = glm::normalize(P1toP2);

	glm::vec3 force (-ks_ * (P1toP2distance - equilibrium_distance_) * P1toP2normalized);

	(*particle1)->force_acc_ -= force;
	(*particle2)->force_acc_ += force;
}



std::string SpringForce::getInfoString() const
{
	std::ostringstream out;

	out << "\tDescription: Spring Force" << std::endl;
	out << "\tStiffness coefficient: " << ks_ << std::endl;
	out << "\tEquilibrium distance: " << equilibrium_distance_ << std::endl;

	return Force::getInfoString() + out.str();
}



DampedSpringForce::DampedSpringForce(f32 ks, f32 kd, f32 equilibrium_distance)
				: Force(TRANSIENT_ON_PARTICLES), ks_(ks), kd_(kd), equilibrium_distance_(equilibrium_distance)
{
}



DampedSpringForce::~DampedSpringForce()
{
}



void DampedSpringForce::apply(const glm::mat4& model, f32 time) const
{
	if (particle_set_.size() != 2)
		std::printf("Spring forces with more than two particles not yet supported\n");

	ParticleSetConstIter iter = particle_set_.begin();
	ParticleSetConstIter particle1 = iter++;
	ParticleSetConstIter particle2 = iter;

	glm::vec3 P1toP2 ((*particle2)->position_ - (*particle1)->position_);
	JU::f32 P1toP2distance = glm::l2Norm(P1toP2);
	glm::vec3 P1toP2normalized = glm::normalize(P1toP2);

	glm::vec3 V1toV2 ((*particle2)->velocity_ - (*particle1)->velocity_);

	glm::vec3 force ((-ks_ * (P1toP2distance - equilibrium_distance_) - kd_ * V1toV2 * P1toP2normalized) * P1toP2normalized);

	(*particle1)->force_acc_ -= force;
	(*particle2)->force_acc_ += force;
}



std::string DampedSpringForce::getInfoString() const
{
	std::ostringstream out;

	out << "\tDescription: Damped Spring Force" << std::endl;
	out << "\tStiffness coefficient: " << ks_ << std::endl;
	out << "\tEquilibrium distance: " << equilibrium_distance_ << std::endl;

	return Force::getInfoString() + out.str();
}



} /* namespace JU */
