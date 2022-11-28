#include "shader.h"
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "renderer.h"
#include <memory.h>

#include "glm/gtc/type_ptr.hpp"

shader::shader(const std::string& filepath) :
    m_filepath(filepath),
    m_rendererId(0)
{
    ShaderSource source = shaderParser(filepath);
    m_rendererId = createShader(source.vertex, source.fragment);
}

shader::~shader() {
    //glDeleteProgram(m_rendererId);
}

void shader::bind() const {
    glUseProgram(m_rendererId);
}

void shader::unbind() const {
    glUseProgram(0);
}


void shader::setUniform1f(const std::string& name, float value) {
    bind();
    glUniform1f(findUniform(name), value);
    //std::cout << "Uniform " << name << " is changed" << std::endl;
}

void shader::setUniform4f(const std::string& name, float f1, float f2, float f3, float f4) {
    bind();
    glUniform4f(findUniform(name), f1, f2, f3, f4);
    std::cout << "Uniform " << name << " is changed" << std::endl;
}

void shader::setUniform3f(const std::string& name, float f1, float f2, float f3) {
    bind();
    glUniform3f(findUniform(name), f1, f2, f3);
    //else std::cout << "Uniform " << name << " is changed" << std::endl;
}

void shader::setUniform3fv(const std::string& name, glm::vec3 value) {
    bind();
    glUniform3f(findUniform(name), value.x, value.y, value.z);
    if (findUniform(name) == 0) std::cout << "Errors with loading uniform " << name << "\n";
    //else std::cout << "Uniform " << name << " is changed" << std::endl;
}

void shader::setUniform1i(const std::string& name, int value) {
    bind();
    glUniform1i(findUniform(name), value);
    //std::cout << "Uniform " << name << " is changed" << std::endl;
}

void shader::setUniformMatrix4f(const std::string& name, const glm::mat4& matrix) {
    bind();
    glUniformMatrix4fv(findUniform(name), 1, GL_FALSE, &matrix[0][0]);
    //if (glGetUniformLocation(m_rendererId, name.c_str()) == -1) std::cout << "Error with loading uniform " << name << std::endl;
    //std::cout << "Matrix uniform " << name << " is changed" << std::endl;
}

void shader::setUniformMatrixModel4f(const std::string& name, const glm::mat4& matrix) {
    if (checkIfUniformExists(name) == false) return;
    bind();
    glUniformMatrix4fv(findUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
    //std::cout << "Matrix uniform " << name << " is changed" << std::endl;
}

bool shader::checkIfUniformExists(const std::string& name)
{
    if (glGetUniformLocation(m_rendererId, name.c_str()) == 0) {
        std::cout << "Failed to find uniform " << name << std::endl;
        return false;
    }
    return true;
}

int shader::findUniform(const std::string& name) {
    if (m_uniform_locations.find(name) != m_uniform_locations.end()) return m_uniform_locations[name];
    int i = glGetUniformLocation(m_rendererId, name.c_str());
    if (i != -1) { 
        m_uniform_locations[name] = i;
        return i;
    }
    else {
        std::cout << "!ERROR! !ERROR! !ERROR! \n"  << " Something went wrong when loading uniform " << name << "\n\n";
        return 0;
    }
}

shader::ShaderSource shader::shaderParser(const std::string& sourcePath)
{
    std::ifstream stream(sourcePath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != -1)
        {
            if (line.find("#shader vertex") != -1)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("#shader fragment") != -1)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
		
		//std::cout << "\n!SHADER DEBUG!\n" << "Shader 1: " << ss[0].str() << "\nShader 2: " << ss[0].str() << "\n"; 

    return { ss[0].str(), ss[1].str() };
}

unsigned int shader::compileShader(const std::string& source, unsigned int type)
{
    const unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //error handling 

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				std::unique_ptr<char[]> message(new char[length]);     
				glGetShaderInfoLog(id, length, &length, message.get());
        std::cout << "Failed to compile shader :( " << std::endl;
        std::cout << message.get() << std::endl;
        glDeleteShader(id);
        return 0;
    }
    else if (result == GL_TRUE) std::cout <<  " Shader " << m_filepath << " is compiled!\n";

    return id;
}

unsigned int shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
