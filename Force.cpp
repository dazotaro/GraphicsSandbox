/*
 * Force.cpp
 *
 *  Created on: Jan 25, 2014
 *      Author: dazotaro
 */

#include "Force.hpp"	// JU::Force

namespace JU
{

Force::Force(bool transient, f32 lifetime) : transient_(transient), lifetime_(lifetime)
{
}



Force::~Force()
{
}



void Force::addParticle(Particle* particle)
{
	particle_list_.push_back(particle);
}



} /* namespace JU */
