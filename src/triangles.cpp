/*!
 * @file 		triangles.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#include "Application.h"

//void frameFunc();
//void DrawGLScene();
//void InitGL();
//using namespace std;
int main(int argc, char **argv)
{
	CAVE::Application app(argc, argv);
	return app.run();
}
//
//void frameFunc()
//{
//  if (CAVEDistribMaster()) {
//	static float prevtime = 0;
//	float t = CAVEGetTime();
//	float dt = t - prevtime;
//	prevtime=t;
//	float jx = CAVE_JOYSTICK_X;
//	float jy = CAVE_JOYSTICK_Y;
//	float _speed_ = 5.0f;
//	if (fabs(jy)>0.2) {
//	float wand[3];
//	CAVEGetVector(CAVE_WAND_FRONT,wand);
//	CAVENavTranslate(wand[0]*jy*dt*_speed_,
//			wand[1]*jy*dt*_speed_,
//			wand[2]*jy*dt*_speed_);
//	}
//	if (fabs(jx) > 0.2) {
//	CAVENavRot(-jx*90*dt,'y');
//	}
//	CAVEDisplayBarrier();
//  }
//}
//
//void InitGL()
//{
//	cout << "OpenGL initialization starting\";
//	if (CAVEMasterDisplay()) {
//		glewInit();
//
//
//	}
//	CAVEDisplayBarrier();
//	glClearColor(.0f, .0f, .0f, .0f);
//	glClearDepth(1.0);
//	glDepthFunc(GL_LESS);
//	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_SMOOTH);
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	cout << "OpenGL initialization finished" << endl;
//}
//
//void draw_triangle(float x, float y, float z)
//{
//	glPushMatrix();
//	glEnable(GL_BLEND);
//	glDisable(GL_DEPTH_TEST);
//
//	glTranslatef(x, y, z);
//	glBegin(GL_TRIANGLES);
//		glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
//		glVertex3f(-1.0f,  4.0f,  -5.0f);
//		glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
//		glVertex3f( 1.0f, 5.0f,  -5.0f);
//		glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
//		glVertex3f( 1.0f,  3.0f,  -5.0f);
//	glEnd();
//	glPopMatrix();
//}
//
//void DrawGLScene()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	CAVENavTransform();
//	draw_triangle(0, 0, 0);
//
//}
//
//
//
//
//
