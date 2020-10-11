#pragma once

#include <string>


class Shader {
private:
	std::string VertexFilePath, FragmentFilePath;
	unsigned int RendererID;

	unsigned int CompileShader(unsigned int type, const std::string filepath);
	unsigned int CreateShader(const std::string& filepathver, const std::string& filepathfrag);

public:
	Shader(const std::string& filepathver, const std::string& filepathfrag);
	~Shader();

	void Bind() const;
	void Unbind() const;

};