//
// Created by srcres on 3/7/24.
//

#include "shader.h"

static char infoLog[8192];

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // 1. Obtain the vertex and fragment shader code from the source files.
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath, std::ifstream::in);
        fShaderFile.open(fragmentPath, std::ifstream::in);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &ex) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        std::cout << "Failed to load shader source." << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. Compile the shaders.
    // Compile the vertex shader.
    unsigned int vertex, fragment;
    int success;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
        std::cout << "Failed to compile the vertex shader, infoLog: " << infoLog << std::endl;
    }
    // Compile the ragment shader.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
        std::cout << "Failed to compile the fragment shader, infoLog: " << infoLog << std::endl;
    }
    // Link the shader program.
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ID, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
        std::cout << "Failed to link the shader program, infoLog: " << infoLog << std::endl;
    }
    // Delete the shaders since they are not used anymore.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    GLint location = glGetUniformLocation(this->ID, name.c_str());
    glUniform1i(location, (int) value);
}

void Shader::setInt(const std::string &name, int value) const
{
    GLint location = glGetUniformLocation(this->ID, name.c_str());
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    GLint location = glGetUniformLocation(this->ID, name.c_str());
    glUniform1f(location, value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    GLint location = glGetUniformLocation(this->ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
