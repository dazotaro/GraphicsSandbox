/*
 * Force.hpp
 *
 *  Created on: Jan 25, 2014
 *      Author: dazotaro
 */

#ifndef FORCE_HPP_
#define FORCE_HPP_

// Local includes
#include "Defs.hpp"						// custom types
#include "ParticleSystemDefs.hpp"		// ParticleList, ForceList

// Global includes
#include <iostream>						// std:;ostream, std::endl
#include <glm/glm.hpp>      	// glm::vec3

namespace JU
{

/*! Force base class */
class Force
{

	public:

		/** Types of forces (based on the way they are eliminated).
		 */
		enum TerminationType
		{
			PERSISTENT,				/**< it has to be explicitly deleted */
			TRANSIENT_ON_PARTICLES,	/**< it is deleted when all the particles it is linked to die */
			TRANSIENT_ON_TIME,		/**< it is deleted when it reaches a maximum lifetime */
		};

		Force(TerminationType type = PERSISTENT, f32 lifetime = 0.0f);
		virtual ~Force();

		virtual void addParticle(Particle* pParticle);
		virtual void apply(f32 time) const = 0;

		void removeParticle(ParticleId particle_id);
		void releaseParticles();

		virtual std::string getInfoString() const;

		friend std::ostream& operator<<(std::ostream& out, const Force& force);

	public:

		ForceId 		id_;
		TerminationType type_;				/**< Is force transient or eternal */
		f32 			lifetime_;			/**< Life left of the force (in milliseconds)*/

		ParticleMap 	particle_map_;		/**< Particle subject to this force */
};



class GravityForce : public Force
{
	public:
		GravityForce(f32 g = 9.80665);
		virtual ~GravityForce();

		void apply(f32 time) const;

		std::string getInfoString() const;

	private:

		f32 g_;		/**< Acceleration due to gravity at earth's surface */
};



class DragForce: public Force
{
	public:
		DragForce(f32 drag = 9.80665);
		virtual ~DragForce();

		void apply(f32 time) const;

		std::string getInfoString() const;

	private:

		f32 drag_;		/**< Acceleration due to gravity at earth's surface */
};



class ThrustForce : public Force
{
	public:
		ThrustForce(const glm::vec3& force, f32 time);
		virtual ~ThrustForce();

		void apply(f32 time) const;

		std::string getInfoString() const;

	private:

		glm::vec3 force_;		/**< Thrust force */
};



class SpringForce : public Force
{
	public:
		SpringForce(f32 stiffness, f32 equilibrium_distance);
		virtual ~SpringForce();

		void apply(f32 time) const;

		std::string getInfoString() const;

	private:

		f32 stiffness_;				/**< Stiffness of the spring to be applied in Hooke's Law */
		f32 equilibrium_distance_;	/**< Distance of equilibrium */
};



} /* namespace JU */
#endif /* FORCE_HPP_ */
