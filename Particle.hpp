/*
 * Particle.hpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

// Local includes
#include "Defs.hpp"			// custom basic types

namespace JU
{

/*! \class Particle
    \brief Base class for all particle types

    It provides all basic information required of a particle
*/class Particle
{
	public:

		Particle(f32 position[3], f32 velocity[3], f32 mass, f32 lifetime);
		virtual ~Particle();

		virtual void render() const = 0;

		f32 position_[3];
		f32 velocity_[3];
		f32 mass_;
		f32 lifetime_;
		f32 forces_[3];
};

} /* namespace JU */
#endif /* PARTICLE_HPP_ */
