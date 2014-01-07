/*!
 * @file 		Triangle.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */
#include <GL/glew.h>
#include "Triangle.h"
#include <iostream>

namespace CAVE {
Triangle::Triangle()
{

}
void Triangle::draw(const point3& position, const color4& color) const
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glTranslatef(position.x, position.y, position.z);
	glBegin(GL_TRIANGLES);
		glColor4fv(&color.r);
		glVertex3f(-0.1f,  -0.1f,  0.0f);
		glColor4fv(&color.r);
		glVertex3f( 0.1f, -0.1f,  0.0f);
		glColor4fv(&color.r);
		glVertex3f( 0.0f,  0.1f,  0.0f);
	glEnd();
	glPopMatrix();
}




}
