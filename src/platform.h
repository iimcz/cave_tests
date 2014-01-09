/*!
 * @file 		platform.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		9.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef CAVE_VERSION
#include <cave_ogl.h>
#else
#include <GL/glut.h>
#endif


namespace CAVE {
inline int get_thread_id() {
#ifdef CAVE_VERSION
	return CAVEUniqueIndex();
#else
	return 0;
#endif

}
}

#endif /* PLATFORM_H_ */
