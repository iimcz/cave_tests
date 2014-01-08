/*!
 * @file 		Application.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Particle.h"
#include <vector>
#include <random>

namespace CAVE {

struct app_state {
	float time_delta 	= 0.0f;
	bool reset_scene 	= false;
	point3 position 	= {0.0f, 0.0f, -5.0f};
	float rotation_y	= 0.0f;
	size_t particles_to_create = 0;
};

class Application {
public:
	Application(int argc, char** argv);
	virtual ~Application() noexcept = default;
	int run();
private:
	void init_gl();
	void update();
	void render() const;
	void update_time(double current_time);
	void reset(bool value);

#ifdef CAVE_VERSION
	void update_cave();
	void init_cave();
#else
	static Application* instance; // This is UGLY and only for GLUT
	static void render_glut();
	static void keyboard_glut(unsigned char key, int x, int y);
#endif


	/*!
	 * Simple helper to catch changes of button value
	 */
	struct button_t {
		bool state = false;
		bool was_pressed = false;
		void update(bool new_state) {
			was_pressed = new_state && !state;
			state = new_state;
		}
	};

	std::vector<Particle> particles_;
	std::mt19937 genenerator_;
	std::uniform_real_distribution<float> distribution_position_;
	std::uniform_real_distribution<float> distribution_direction_;
	double last_time_;
	app_state state_;
	std::vector<button_t> buttons_;
};

}




#endif /* APPLICATION_H_ */
