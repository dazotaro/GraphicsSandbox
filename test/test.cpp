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
#include <JU/Defs.hpp>

// Global includes
#include <iostream>



/**
* It tests:
* 	- Adds Force objects to Particle objects
* 	- Adds Particle objects to Force objects
*/
void testForce01()
{
	std::map<std::string, JU::Force*> force_map;

	force_map["spring01"] = new JU::GravityForce();
	force_map["gravity"]  = new JU::GravityForce();
	force_map["friction"] = new JU::DragForce	(427.2f);
	force_map["spring02"] = new JU::GravityForce();

	glm::vec3 pos(1.1f, 2.2f, 3.3f);
	glm::vec3 vel(7.1f, 7.2f, 7.3f);
	JU::Particle particle(333.3, pos, vel, 13);
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



/**
* It tests:
* 	- Adds Force objects to Particle objects
* 	- Adds Particle objects to Force objects
* 	- Adds Force objects to a ParticleSystem object
* 	- Adds Particle objects to a ParticleSystem object
*/
void testForce02()
{
	JU::ParticleSystem particle_system;

	std::map<std::string, JU::Force*> force_map;

	force_map["spring01"] = new JU::GravityForce();
	force_map["gravity"]  = new JU::GravityForce();
	force_map["friction"] = new JU::DragForce(427.2f);
	force_map["spring02"] = new JU::GravityForce();

	glm::vec3 pos (1.1f, 2.2f, 3.3f);
	glm::vec3 vel (7.1f, 7.2f, 7.3f);
	JU::Particle* particle = new JU::Particle(333.3, pos, vel, 13);
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



/**
* It tests:
* 	- Adds Force objects to Particle objects
* 	- Adds Particle objects to Force objects
* 	- Adds Force objects to a ParticleSystem object
* 	- Adds several Particle objects to a ParticleSystem object
*/
void testForce03()
{
	JU::ParticleSystem particle_system;

	// PARTICLE SYSTEM: add forces
	// ---------------------------
	std::map<std::string, JU::ForceId> force_map;

	force_map["spring01"] = particle_system.addForce(new JU::SpringForce(0.8f, 2.0f));
	force_map["gravity"]  = particle_system.addForce(new JU::GravityForce());
	force_map["friction"] = particle_system.addForce(new JU::DragForce(427.2f));
	force_map["spring02"] = particle_system.addForce(new JU::SpringForce(0.3f, 2.0f));
	force_map["thrust"]   = particle_system.addForce(new JU::ThrustForce(glm::vec3(1.0f, 200.0f, 3.0f), 5.0f));

	// PARTICLE SYSTEM: add particles
	// ------------------------------
	JU::Particle* particle;
	JU::ForceIdVector force_vec(4);

	// PARTICLE 01
	// -----------
	glm::vec3 pos1 (1.1f, 2.2f, 3.3f);
	glm::vec3 vel1 (7.1f, 7.2f, 7.3f);
	particle = new JU::Particle(111.1, pos1, vel1, 1000);

	// Compile list of force ids that control this particle
	force_vec.clear();
	force_vec.push_back(force_map["gravity"]);
	force_vec.push_back(force_map["friction"]);
	force_vec.push_back(force_map["spring01"]);
	force_vec.push_back(force_map["thrust"]);

	particle_system.addParticle(particle, force_vec);

	// PARTICLE 02
	// -----------
	glm::vec3 pos2 (4.1f, 5.2f, 6.3f);
	glm::vec3 vel2 (9.1f, 9.2f, 9.3f);
	particle = new JU::Particle(222.2, pos2, vel2, 5000);

	// Compile list of force ids that control this particle
	force_vec.clear();
	force_vec.push_back(force_map["gravity"]);
	force_vec.push_back(force_map["friction"]);
	force_vec.push_back(force_map["spring02"]);
	force_vec.push_back(force_map["thrust"]);

	particle_system.addParticle(particle, force_vec);

	std::cout << "Printing the whole particle system" << std::endl;
	std::cout << particle_system << std::endl;
}



int main(void)
{
	testForce03();

	return 0;
}
