//
// Created by srcres on 3/7/24.
//

#ifndef GLFW_LEARN_SHADER_H
#define GLFW_LEARN_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint ID;

    Shader(const char *vertexPath, const char *fragmentPath);

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif //GLFW_LEARN_SHADER_H
