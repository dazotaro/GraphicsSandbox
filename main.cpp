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
	std::map<std::string, JU::Force*> force_map;

	force_map["spring01"] = new JU::Force(0, true, 1111);
	force_map["gravity"]  = new JU::Force(1, false, 2222);
	force_map["friction"] = new JU::Force(2, false, 3333);
	force_map["spring02"] = new JU::Force(3, true, 4444);

	JU::f32 pos[3] = {1.1f, 2.2f, 3.3f};
	JU::f32 vel[3] = {7.1f, 7.2f, 7.3f};
	JU::Particle particle(17, 333.3, pos, vel, 13);
	particle.addForce(force_map["spring01"]);
	particle.addForce(force_map["gravity"]);
	particle.addForce(force_map["friction"]);

	force_map["spring01"]->addParticle(&particle);
	force_map["gravity"]->addParticle(&particle);
	force_map["friction"]->addParticle(&particle);

	// Print particles
	std::cout << particle << std::endl;

	// Print forces
	std::map<std::string, JU::Force*>::iterator force_map_iter = force_map.begin();
	for (;force_map_iter != force_map.end(); force_map_iter++)
	{
		std::cout << *(force_map_iter->second) << std::endl;
	}

	std::cout << "Detach particle and gravity" << std::endl;
	force_map["gravity"]->removeParticle(particle.id_);
	std::cout << *force_map["gravity"] << std::endl;

	// Delete forces
	force_map_iter = force_map.begin();
	for (;force_map_iter != force_map.end(); force_map_iter++)
	{
		delete force_map_iter->second;
	}

}



void testForce02()
{
	JU::ParticleSystem particle_system;

	std::map<std::string, JU::Force*> force_map;

	JU::Force* pForce = new JU::Force(0, true, 1111);
	force_map["spring01"] = pForce;
	force_map["gravity"]  = new JU::Force(1, false, 2222);
	force_map["friction"] = new JU::Force(2, false, 3333);
	force_map["spring02"] = new JU::Force(3, true, 4444);

	JU::f32 pos[3] = {1.1f, 2.2f, 3.3f};
	JU::f32 vel[3] = {7.1f, 7.2f, 7.3f};
	JU::Particle* particle = new JU::Particle(17, 333.3, pos, vel, 13);
	particle->addForce(force_map["spring01"]);
	particle->addForce(force_map["gravity"]);
	particle->addForce(force_map["spring02"]);

	force_map["spring01"]->addParticle(particle);
	force_map["gravity"]->addParticle(particle);
	force_map["friction"]->addParticle(particle);

	// Add FORCES to the system
	particle_system.addForce(force_map["spring01"]);
	particle_system.addForce(force_map["gravity"]);
	particle_system.addForce(force_map["friction"]);
	particle_system.addForce(force_map["spring02"]);

	// Add particles to the system
	particle_system.addParticle(particle);

	std::cout << "Printing the whole particle system" << std::endl;
	std::cout << particle_system << std::endl;
}



int main(void)
{
	testForce02();

	return 0;
}
