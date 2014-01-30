//============================================================================
// Name        : particle_system.cpp
// Author      : dazotaro
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

// Local includes
#include "Force.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "Defs.hpp"

// Global includes
#include <iostream>

void testForce01()
{
	JU::Force force(0, true, 1111);

	JU::f32 pos[3] = {1.1f, 2.2f, 3.3f};
	JU::f32 vel[3] = {7.1f, 7.2f, 7.3f};
	JU::Particle particle(17, 333.3, pos, vel, 13);

	force.addParticle(&particle);
	particle.addForce(&force);

	std::cout << force << std::endl;
	std::cout << particle << std::endl;

	force.removeParticle(particle.id_);
	std::cout << force << std::endl;
}

int main(void)
{
	testForce01();

	return 0;
}
