/*
 * Particle.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

// Local includes
#include <JU/core/Defs.hpp>			// Custom basic types
#include "ParticleSystemDefs.hpp"	// ParticleId

// Global includes
#include <iostream>			// std:;ostream, std::endl
#include <glm/glm.hpp>          	// glm::vec3

namespace JU
{

// Forward Declarations
//class Force;

/*! \class Particle
    \brief Base class for all particle types

    It provides all basic information required of a particle
*/
class Particle
{
	public:

		Particle(f32 mass, const glm::vec3& position, const glm::vec3& velocity, f32 lifetime);
		virtual ~Particle();

		void resetForceAcc();
		void addForce(Force* pForce);
		void removeForce(Force* pForce);
		void releaseForces();

		// Friend functions
		friend std::ostream& operator<<(std::ostream& out, const Particle& particle);

	public:

		ParticleId id_;					/**< particle id (for debugging purposes */

		f32 	  mass_;
		glm::vec3 position_;
		glm::vec3 velocity_;
		glm::vec3 force_acc_;		/**< Force accumulator*/

		f32 lifetime_;					/**< Life left of the force (in milliseconds) */

		ForceSet force_set_;	/**< To store pointer to forces acting on this particle */
};

} /* namespace JU */
#endif /* PARTICLE_HPP_ */
