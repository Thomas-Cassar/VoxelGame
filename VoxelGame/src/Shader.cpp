#include "Shader.h"

#include "Renderer.h"

#include <fstream>
#include <iostream>

Shader::Shader(const std::string& filepathver, const std::string& filepathfrag)
	:VertexFilePath(filepathver),
	FragmentFilePath(filepathfrag),
	RendererID(0)
{
	RendererID = CreateShader(VertexFilePath,FragmentFilePath);
}

Shader::~Shader()
{
}

unsigned int Shader::CompileShader(unsigned int type, const std::string filepath)
{
	std::ifstream file(filepath);
	if (file.fail())
	{
		std::cerr << "Could not open " << filepath << std::endl;
		exit(1);
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	std::string filebuffer(size, ' ');
	file.seekg(0);
	file.read(&filebuffer[0], size);

	unsigned int id = glCreateShader(type);
	const char* src = filebuffer.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	file.close();
	return id;
}

unsigned int Shader::CreateShader(const std::string& filepathver, const std::string& filepathfrag)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, filepathver);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, filepathfrag);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


void Shader::Bind() const
{
	GLCall(glUseProgram(RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}
