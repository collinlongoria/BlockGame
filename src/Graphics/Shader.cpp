//
// Created by colli on 12/26/2024.
//

#include "Graphics/Shader.hpp"
#include "Core/Log.hpp"

#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode, fragmentCode;

    // Vertex shader
    std::ifstream vertexShaderFile;

    vertexShaderFile.open(vertexPath);
    if(!vertexShaderFile.is_open()) {
        Log::Output(Log::Severity::ERROR, "Could not open vertex shader file.");
    }
    else {
        std::stringstream vertexShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        vertexShaderFile.close();
        vertexCode = vertexShaderStream.str();
    }

    // Fragment shader
    std::ifstream fragmentShaderFile;

    fragmentShaderFile.open(fragmentPath);
    if(!fragmentShaderFile.is_open()) {
        Log::Output(Log::Severity::ERROR, "Could not open fragment shader file.");
    }
    else {
        std::stringstream fragmentShaderStream;
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        fragmentShaderFile.close();
        fragmentCode = fragmentShaderStream.str();
    }

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar *vertexSource = vertexCode.c_str();
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        Log::Output(Log::Severity::ERROR, "Vertex Shader Compilation Failed: " + std::string(infoLog));
    }

    const GLchar *fragmentSource = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        Log::Output(Log::Severity::ERROR, "Fragment Shader Compilation Failed:b" + std::string(infoLog));
    }

    program = glCreateProgram();

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        Log::Output(Log::Severity::ERROR, "Shader Program Link Failed: " + std::string(infoLog));
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    Log::Output(Log::Severity::HAPPY, "A Shader has been created.");
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::Bind() {
    glUseProgram(program);
}


