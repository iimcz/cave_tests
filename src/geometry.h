/*!
 * @file 		geometry.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include <algorithm>
namespace CAVE {
struct point3 {
	float x;
	float y;
	float z;
};
struct color4 {
	float r;
	float g;
	float b;
	float a;
};

template<typename T>
point3 operator*(const T& scalar, const point3& point)
{
	return {point.x * scalar, point.y * scalar, point.z * scalar};
}
template<typename T>
point3 operator*(const point3& point, const T& scalar)
{
	return {point.x * scalar, point.y * scalar, point.z * scalar};
}
inline point3 operator+(const point3& point1, const point3& point2)
{
	return {point1.x + point2.x, point1.y + point2.y, point1.z + point2.z};
}

inline color4 color_grad(const color4& start, const color4 end, float val)
{
	val = std::max(0.0f, std::min(val,1.0f));
	float val1 = 1.0 - val;
	return {
		start.r * val1 + end.r * val,
		start.g * val1 + end.g * val,
		start.b * val1 + end.b * val,
		start.a * val1 + end.a * val
	};
}

}


#endif /* GEOMETRY_H_ */
