/*!
 * @file 		Scene.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		9.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */
#include "Scene.h"
#include "platform.h"
#include <stdexcept>
namespace CAVE {


namespace {
/*!
 * Creates new particle using provided generator and distribution objects.
 * @param d_position  Distribution object for generating particle position
 * @param d_direction Distribution object for generating particle direction
 * @param generator   Generator providing random numbers
 * @return
 */
template<class Generator>
Particle create_particle(std::uniform_real_distribution<float>& d_position,
		std::uniform_real_distribution<float>& d_direction,
		Generator& generator)
{
	point3 position;
	position.x = d_position(generator);
	position.y = d_position(generator);
	position.z = d_position(generator);
	point3 direction;
	direction.x = d_direction(generator);
	direction.y = d_direction(generator)*2.0f+2.0;
	direction.z = d_direction(generator);
	return {position, direction};
}
const std::string fragment_shader = R"XXX(
		#version 120
		//out vec4 color;
		uniform vec4 color_in;
		void main() {
			//color	= vec4(1.0, 0.0, 0.0, 1.0);
			float dist = distance(gl_TexCoord[0].xy,vec2(0.5,0.5));
			if (dist > 0.5) {
				discard;
			} else {
				float val = (1.0 - 2 * dist);
				gl_FragColor = vec4(sqrt(val) * gl_Color.xyz, val*val);
			}
		}
)XXX";

void draw_quad(const point3& position, const color4& color)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslatef(position.x, position.y, position.z);
	glBegin(GL_QUADS);
		glColor4fv(&color.r);
		glTexCoord2f(0.0f,  0.0f);
		glVertex3f(-0.1f,  -0.1f,  0.0f);
		glTexCoord2f(1.0f,  0.0f);
		glColor4fv(&color.r);
		glVertex3f( 0.1f, -0.1f,  0.0f);
		glTexCoord2f(1.0f,  1.0f);
		glColor4fv(&color.r);
		glVertex3f( 0.1f,  0.1f,  0.0f);
		glTexCoord2f(0.0f,  1.0f);
		glColor4fv(&color.r);
		glVertex3f(-0.1f,  0.1f,  0.0f);
	glEnd();
	glPopMatrix();
}
}







Scene::Scene(size_t particles_per_second):
particles_per_second_(particles_per_second),
distribution_position_(0.0, 1.0), distribution_direction_(-1.0, 1.0)
{

}


void Scene::update(float time_delta)
{
	size_t particles_to_create = particles_per_second_ * time_delta;
	for (size_t i = 0; i < particles_to_create; ++i) {
		particles_.emplace_back(create_particle(distribution_position_, distribution_direction_, generator_));
	}
	for (auto& p: particles_) {
		p.update(time_delta);
	}
	particles_.erase(std::remove_if(particles_.begin(), particles_.end(),
			[](Particle& p){return p.dead();}), particles_.end());
}
void Scene::render(const point3& position, const float rotation_y) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * For the sake of simplicity, the old OpenGL matrix stack is used here.
	 *
	 * In OpenGL 3.0, where the matrices are handled in shaders,
	 * will things get a little bit more complicated.
	 *
	 * As CAVElib is not aware of OpenGL 3.0+, it always uses the matrix stack.
	 * So in order to get the matrices, we have to read them from the stack.
	 * For example:
	 *
	 * std::array<float, 16> modelview_matrix;
	 * glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix.data());
	 * std::array<float, 16> projection_matrix;
	 * glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.data());
	 *
	 */

	glRotatef(-rotation_y  * 180.0f / pi_constant, 0.0f, 1.0f, 0.0f);
	glTranslatef(position.x, position.y, position.z);


	const gl_details_t& detail = details_.at(get_thread_id());
	detail.shader.bind();

	// The important part here is that particles are handled only through const references.
	// And the vector is never modified.
	for (const auto& p: particles_) {
		draw_quad(p.position, p.get_color());
	}
	detail.shader.unbind();
}
void Scene::reset()
{
	particles_.clear();
}

void Scene::set_seed(unsigned int seed)
{
	generator_.seed(seed);
}

void Scene::prepare_details()
{
	const int thread_id = get_thread_id();
	if (details_.find(thread_id) != details_.end()) {
		throw std::runtime_error("Attemt to initialize already initialized detail!");
	}

	details_.insert(std::make_pair(thread_id, gl_details_t(fragment_shader, "")));
}


}
