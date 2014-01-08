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
const color4 cold {0.0f, 0.73f, 1.0f, 1.0f};
const color4 hot {0.8f, 0.0f, 0.0f, 1.0f};
const float default_life = 10.0f;
const float slowdown_per_second = 0.2f;
}

Particle::Particle(const point3& position, const point3& direction):
life_(default_life),position_(position), direction_(direction)
{

}

void Particle::draw() const
{
	if (life_ > 0.0f)
		triangle_.draw(position_,color_grad(cold, hot, direction_.y/2+1.0f));
}
void Particle::update(float time_delta)
{
	position_ = position_ + (time_delta*direction_);
	direction_ = (1.0f - time_delta * slowdown_per_second) * direction_ + time_delta * gravity;
	life_ -= time_delta;
}

}



