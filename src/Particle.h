/*!
 * @file 		Particle.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#ifndef PARTICLE_H_
#define PARTICLE_H_
#include "Triangle.h"

namespace CAVE {
class Particle {
public:
	Particle(const point3& position, const point3& direction);
	~Particle() noexcept = default;
	void draw() const;
	bool dead() const { return life_ < 0;}
	void update(float time_delta);
private:
	float life_;
	point3 position_;
	point3 direction_;
	Triangle triangle_;
};


}



#endif /* PARTICLE_H_ */
