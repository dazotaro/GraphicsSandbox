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

		Particle(f32 mass,
				 const glm::vec3& position,
				 const glm::vec3& velocity,
				 const glm::vec4& color,
				 bool update = true,
				 bool is_inmortal = true,
				 JU::f32 lifetime = 0.0f);
		virtual ~Particle();

		void resetForceAcc();

		// Friend functions
		friend std::ostream& operator<<(std::ostream& out, const Particle& particle);

	public:

		ParticleId id_;					/**< particle id (for debugging purposes */

		f32 	  mass_;
		glm::vec3 position_;
		glm::vec3 velocity_;
		glm::vec3 force_acc_;		/**< Force accumulator */
		glm::vec4 color_;
		bool update_;				/**< Exclude this particle from the integrator? */

		bool is_inmortal_;
		f32 lifetime_;					/**< Life left of the force (in milliseconds) */
};

} /* namespace JU */
#endif /* PARTICLE_HPP_ */
