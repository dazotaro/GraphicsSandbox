/*
 * Particle.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

// Local includes
#include "Defs.hpp"					// Custom basic types
#include "ParticleSystemDefs.hpp"	// ParticleId

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

		Particle(ParticleId id, f32 position[3], f32 velocity[3], f32 mass, f32 lifetime);
		virtual ~Particle();

		void resetForceAcc();
		void addForce(Force* pForce);
		void removeForce(Force* pForce);
		void releaseForces();

		virtual void render() const = 0;

	public:

		ParticleId id_;					/**< particle id (for debugging purposes */

		f32 position_[3];
		f32 velocity_[3];
		f32 mass_;
		f32 lifetime_;					/**< Life left of the force (in milliseconds) */
		f32 force_acc_[3];				/**< Force accumulator*/

		ForceSet force_set_;	/**< To store pointer to forces acting on this particle */
};

} /* namespace JU */
#endif /* PARTICLE_HPP_ */
