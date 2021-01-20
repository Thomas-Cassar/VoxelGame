#pragma once

#include <string>
#include <unordered_map>
#include "glm\glm.hpp"


class Shader {
private:
	std::string VertexFilePath, FragmentFilePath;
	unsigned int RendererID;

	unsigned int CompileShader(unsigned int type, const std::string filepath);
	unsigned int CreateShader(const std::string& filepathver, const std::string& filepathfrag);
	std::unordered_map<std::string, int> UniformLocationCache;

public:
	Shader(const std::string& filepathver, const std::string& filepathfrag);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float f2);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	int GetUniformLocation(const std::string& name);
};