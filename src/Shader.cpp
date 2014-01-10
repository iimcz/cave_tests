/*!
 * @file 		Shader.cpp
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		7.1.2014
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */
#include "Shader.h"
#include <stdexcept>
#include <iostream>

namespace CAVE {

ShaderProgram::Shader::Shader(const std::string& text, GLenum type):shader(0)
{
	if (text.empty()) return;
	shader = glCreateShader(type);
	const GLchar *source = text.c_str();
	GLint text_size = text.size();
	glShaderSource(shader,1,reinterpret_cast<const GLchar**>(&source),&text_size);
	//shader_text.resize(shader_size);
	glCompileShader(shader);
	GLint compiled;
	glGetObjectParameterivARB(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint blen = 0;
		GLsizei slen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &blen);
		if (blen > 1)
		{
		 std::string compiler_log(blen,0);
		 glGetInfoLogARB(shader, blen, &slen, &compiler_log[0]);
		 std::cerr << "compiler_log:" <<  compiler_log <<"\n";
		}
		throw std::runtime_error("Failed to compile shader");
	}
}

ShaderProgram::ShaderProgram(const std::string& vertex_shader_text,
							const std::string& fragment_shader_text,
							const std::string& geometry_shader_text)
{
	Shader vertex_shader(vertex_shader_text, GL_VERTEX_SHADER);
	Shader fragment_shader(fragment_shader_text, GL_FRAGMENT_SHADER);
	Shader geometry_shader(geometry_shader_text, GL_GEOMETRY_SHADER);
	program_ = glCreateProgram();
	if (vertex_shader.shader) glAttachShader(program_, vertex_shader.shader);
	if (fragment_shader.shader) glAttachShader(program_, fragment_shader.shader);
	if (geometry_shader.shader) glAttachShader(program_, geometry_shader.shader);
}

void ShaderProgram::bind() const
{
	glUseProgram(program_);
}
void ShaderProgram::unbind() const
{
	glUseProgram(0);
}
bool ShaderProgram::link()
{
	glLinkProgram(program_);
	GLint linked;
	glGetProgramiv(program_, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint blen = 0;
		GLsizei slen = 0;
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH , &blen);
		if (blen > 1)
		{
			std::string compiler_log(blen,0);
			glGetInfoLogARB(program_, blen, &slen, &compiler_log[0]);
			std::cerr << "compiler_log:" <<  compiler_log << "\n";
		}
		return false;
	}
	return true;
}

void ShaderProgram::bind_attrib(GLuint index, const std::string& name)
{
	glBindAttribLocation(program_,index,name.c_str());
}
void ShaderProgram::bind_frag_data(GLuint index, const std::string& name)
{
	glBindFragDataLocation(program_, index, name.c_str());
}

namespace {

template<class F>
bool set_uniform_generic(const std::string& name, GLuint program, F execute) {
	glGetError();
	GLint loc = glGetUniformLocation(program,name.c_str());
	if (loc < 0) return false;
	execute(loc);
	if (glGetError()) return false;
	return true;
}
}

bool ShaderProgram::set_uniform_matrix4(const std::string& name,const glm::mat4& matrix)
{
	return set_uniform_generic(name, program_, [matrix](GLint loc){glUniformMatrix4fv(loc,1,GL_FALSE,&matrix[0][0]);});
}

}




