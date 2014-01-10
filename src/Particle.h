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
//#include "Triangle.h"
#include "geometry.h"

namespace CAVE {
struct Particle {
public:
	Particle(const point3& position, const point3& direction);
	~Particle() noexcept = default;
	bool dead() const { return life < 0;}
	void update(float time_delta);
	color4 get_color() const;


	point3 position;
	point3 direction;
	float life;

};


}



#endif /* PARTICLE_H_ */
