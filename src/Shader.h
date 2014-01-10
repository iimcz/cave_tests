/*!
 * @file 		Shader.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */


#ifndef SHADER_H_
#define SHADER_H_
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace CAVE {


class ShaderProgram {
	struct Shader {
		Shader(const std::string& text, GLenum type);
		GLuint shader;
	};
public:
	ShaderProgram(const std::string& vertex_shader_text, const std::string& fragment_shader_text, const std::string& geometry_shader_text = std::string());
	bool link();
	void bind_attrib(GLuint index, const std::string& name);
	void bind_frag_data(GLuint index, const std::string& name);
	void bind() const;
	void unbind() const;

	bool set_uniform_matrix4(const std::string& name,const glm::mat4& matrix);
private:
	GLuint program_;

};

}


#endif /* SHADER_H_ */
