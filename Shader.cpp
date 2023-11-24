#include "Shader.hpp"



Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
	string vertexCode, fragmentCode;
	ifstream vertexFile, fragmentFile;

	// Ensure ifstream objects can throw exceptions
	vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// Open files
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		// Read file's buffer contents into streams
		stringstream vertexStream, fragmentStream;
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		// Close files
		vertexFile.close();
		fragmentFile.close();
		// Convert streams to strings
		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (ifstream::failure f)
	{
		std::cerr << "ERROR::SHADER::FILE_COULD_NOT_BE_READ" << endl;
	}

	// Convert strings to C-strings
	const GLchar *vertexShaderSource = vertexCode.c_str();
	const GLchar *fragmentShaderSource = fragmentCode.c_str();



	GLint success;
	GLchar infoLog[512];

	// Compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Print compile errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Print compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Build shader program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	// Print linking errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Delete Shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Shader::use() const
{
	glUseProgram(id);
}


void Shader::setUniform(const string &name, GLboolean value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, (GLint)value);
}


void Shader::setUniform(const string &name, GLint value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1i(location, value);
}


void Shader::setUniform(const string &name, GLfloat value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform1f(location, value);
}


void Shader::setUniform(const string &name, vec3 value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}


void Shader::setUniform(const string &name, mat4 value) const
{
	GLint location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}