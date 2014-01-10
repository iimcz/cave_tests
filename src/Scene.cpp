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
#include <iostream>
#include <GL/glu.h>

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
		#version 150
		in vdata {
			vec2 texcoords;
			vec4 color;
		} vtx;
		out vec4 color;
		void main() {
			//color = vec4(1.0,0.0,0.0,1.0);
			//return;
			float dist = distance(vtx.texcoords.xy,vec2(0.5,0.5));
			if (dist > 0.5) {
				discard;
			} else {
				float val = (1.0 - 2 * dist);
				color = vec4(sqrt(val) * vtx.color.xyz, val*val*val);
			}
		}
)XXX";
const std::string vertex_shader = R"XXX(
		#version 120
		#extension GL_ARB_gpu_shader5 : enable
		vec4 cold = vec4(0.0f, 0.73f, 1.0f, 1.0f);
		vec4 hot = vec4(0.8f, 0.0f, 0.0f, 1.0f);
		in vec3 position;
		in vec3 direction;

		out vdata0 {
			vec4 color;
		} vertex;

		void main() {
			gl_Position = gl_ModelViewProjectionMatrix * vec4(position.xyz, 1.0);
			vertex.color = mix(cold, hot, clamp(direction.y,-1.0,1.0)/2+0.5);
		}
)XXX";

const std::string geometry_shader = R"XXX(
		#version 150
		layout (points) in;
		layout (triangle_strip, max_vertices=4) out;

		uniform float size = 0.5;
		
		in vdata0 {
			vec4 color;
		} vertex[];

		out vdata {
			vec2 texcoords;
			vec4 color;
		} vtx;

		void main() {
			vtx.texcoords = vec2(-1, -1);
			vtx.color = vertex[0].color;
			gl_Position = gl_in[0].gl_Position + vec4(vtx.texcoords * size, 0.0,0.0);
			EmitVertex();
			vtx.texcoords = vec2(1, -1);
			vtx.color = vertex[0].color;
			gl_Position = gl_in[0].gl_Position + vec4(vtx.texcoords * size, 0.0,0.0);
			EmitVertex();
			vtx.texcoords = vec2(-1, 1);
			vtx.color = vertex[0].color;
			gl_Position = gl_in[0].gl_Position + vec4(vtx.texcoords * size, 0.0,0.0);
			EmitVertex();
			vtx.texcoords = vec2(1, 1);
			vtx.color = vertex[0].color;
			gl_Position = gl_in[0].gl_Position + vec4(vtx.texcoords * size, 0.0,0.0);
			EmitVertex();
			EndPrimitive();
		}
)XXX";

bool check_gl_error(const std::string& file, size_t line) {
	GLuint glerr;
	if ((glerr=glGetError())) {
		std::cerr  << "opengl error: " << gluErrorString(glerr) << " at " << file <<":"<< line;
		return false;
	}
	return true;
}
#define GL_CHECK_ERROR check_gl_error(__FILE__, __LINE__);

//void draw_quad(const point3& position, const color4& color)
//{
//	glPushMatrix();
//	glEnable(GL_BLEND);
//	glDisable(GL_DEPTH_TEST);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glTranslatef(position.x, position.y, position.z);
//	glBegin(GL_QUADS);
//		glColor4fv(&color.r);
//		glTexCoord2f(0.0f,  0.0f);
//		glVertex3f(-0.1f,  -0.1f,  0.0f);
//		glTexCoord2f(1.0f,  0.0f);
//		glColor4fv(&color.r);
//		glVertex3f( 0.1f, -0.1f,  0.0f);
//		glTexCoord2f(1.0f,  1.0f);
//		glColor4fv(&color.r);
//		glVertex3f( 0.1f,  0.1f,  0.0f);
//		glTexCoord2f(0.0f,  1.0f);
//		glColor4fv(&color.r);
//		glVertex3f(-0.1f,  0.1f,  0.0f);
//	glEnd();
//	glPopMatrix();
//}
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


	const gl_details_t& detail = get_detail();
	detail.shader.bind();

	// The important part here is that particles are handled only through const references.
	// And the vector is never modified.
//	for (const auto& p: particles_) {
//		draw_quad(p.position, p.get_color());
//	}
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(detail.vba);
	glBindBuffer(GL_ARRAY_BUFFER, detail.fbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*particles_.size(), &particles_[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, particles_.size());
	glBindVertexArray(0);
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
	const GLuint index_vertices = 0;
	const GLuint index_directions = 1;
	const std::string name_vertices = "position";
	const std::string name_direction = "direction";


	gl_details_t& detail = new_detail();


	detail.shader.bind_attrib(index_vertices, name_vertices);
	GL_CHECK_ERROR
	detail.shader.bind_attrib(index_directions, name_direction);
	GL_CHECK_ERROR
	detail.shader.bind_frag_data(0, "color");
	GL_CHECK_ERROR
	detail.shader.link();
	GL_CHECK_ERROR

	static_assert(sizeof(Particle) == 7*sizeof(float),"Wrong padding of Particle!");

	glGenVertexArrays(1, &detail.vba);
	GL_CHECK_ERROR
	glBindVertexArray(detail.vba);
	GL_CHECK_ERROR

	glGenBuffers(1, &detail.fbo);

	glBindBuffer(GL_ARRAY_BUFFER, detail.fbo);
	GL_CHECK_ERROR
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);


	glEnableVertexAttribArray(index_vertices);
	GL_CHECK_ERROR
	glVertexAttribPointer(index_vertices, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	GL_CHECK_ERROR

	glEnableVertexAttribArray(index_directions);
	GL_CHECK_ERROR
	glVertexAttribPointer(index_directions, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const void*>(3*sizeof(float)));
	GL_CHECK_ERROR

	glBindVertexArray(0);
}

Scene::gl_details_t& Scene::new_detail()
{
	std::unique_lock<std::mutex> _(detail_mutex_);
	const int thread_id = get_thread_id();
	if (details_.find(thread_id) != details_.end()) {
		throw std::runtime_error("Attemt to initialize already initialized detail!");
	}

	auto res = details_.insert(std::make_pair(thread_id, gl_details_t(fragment_shader, vertex_shader, geometry_shader)));
	assert(res.second);
	return res.first->second;
}

const Scene::gl_details_t& Scene::get_detail() const
{
	std::unique_lock<std::mutex> _(detail_mutex_);
	return details_.at(get_thread_id());
}

}


