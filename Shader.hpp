#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;



class Shader
{
	private:
		GLuint id;

	public:
		// Read the GLSL code from the specified files and build the shader program
		Shader(const char* vertexPath, const char* fragmentPath);

		// Activate this shader program
		void use() const;

		// Set uniform
		void setUniform(const string &name, GLboolean value) const;
		void setUniform(const string &name, GLint value) const;
		void setUniform(const string &name, GLfloat value) const;
		void setUniform(const string &name, vec3 value) const;
		void setUniform(const string &name, mat4 value) const;
};