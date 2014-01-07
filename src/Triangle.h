/*!
 * @file 		triangle.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include "geometry.h"
#include <GL/gl.h>

namespace CAVE {
	class Triangle {
	public:
		Triangle();
		~Triangle() noexcept = default;
		void draw(const point3& position, const color4& color) const;
	};
}






#endif /* TRIANGLE_H_ */
