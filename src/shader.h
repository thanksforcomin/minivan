#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

class shader {
private:
	unsigned int m_rendererId;
	std::string m_filepath;

	struct ShaderSource
	{
		std::string vertex;
		std::string fragment;
	};

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::unordered_map<std::string, int> m_uniform_locations;

	unsigned int compileShader(const std::string& source, unsigned int type);
  unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderSource shaderParser(const std::string& sourcePath);
	bool checkIfUniformExists(const std::string& name);
	int findUniform(const std::string& name);
public:
	shader(const std::string& filepath);
	~shader();

	void bind() const;
	void unbind() const;

	void setUniform3f(const std::string& name, float f1, float f2, float f3);
	void setUniform3fv(const std::string& name, glm::vec3 value);
	void setUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	void setUniform1f(const std::string& name, float value);
	void setUniform1i(const std::string& name, int value);
	void setUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
	void setUniformMatrixModel4f(const std::string& name, const glm::mat4& matrix);
	//void setUniformBuffer(const std::string& name)
};
