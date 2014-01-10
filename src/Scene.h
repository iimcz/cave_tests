/*!
 * @file 		Scene.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		9.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#ifndef SCENE_H_
#define SCENE_H_
#include "Particle.h"
#include "Shader.h"
#include <random>
#include <vector>
#include <map>
#include <mutex>

namespace CAVE {
	class Scene {
	public:
		Scene(size_t particles_per_second);
		~Scene() noexcept = default;
		void update(float time_delta);
		void render(const point3& position, const float rotation_y) const;
		void reset();
		void set_seed(unsigned int seed);
		void prepare_details();
	private:
		size_t particles_per_second_;
		std::vector<Particle> particles_;
		std::mt19937 generator_;
		std::uniform_real_distribution<float> distribution_position_;
		std::uniform_real_distribution<float> distribution_direction_;

		struct gl_details_t{
			gl_details_t(const std::string& fs, const std::string& vs, const std::string& gs=std::string()):
			shader(vs,fs,gs),vba(0),fbo(0) {	}

			ShaderProgram shader;
			GLuint vba;
			GLuint fbo;

		};
		std::map<int, gl_details_t> details_;
		mutable std::mutex detail_mutex_;


		gl_details_t& new_detail();
		const gl_details_t& get_detail() const;
	};


}



#endif /* SCENE_H_ */
