/*!
 * @file 		Particle.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Particle.h"
namespace CAVE {

namespace {
const point3 gravity {0.0f, -1.0f, 0.0f};
const float default_life = 10.0f;
const float slowdown_per_second = 0.2f;
}

Particle::Particle(const point3& position, const point3& direction):
position(position), direction(direction),life(default_life)
{

}

void Particle::update(float time_delta)
{
	position = position + (time_delta*direction);
	direction = (1.0f - time_delta * slowdown_per_second) * direction + time_delta * gravity;
	life -= time_delta;
}

}



