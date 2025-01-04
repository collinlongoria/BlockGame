//
// Created by colli on 12/26/2024.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>

#include <Core/Math.hpp>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void Bind();

    // Uniform wrapper helper function
    template<typename T>
    void SetUniform(const std::string& name, const T& value) {
        if constexpr(std::is_same_v<T, Mat4>) {
            glUniformMatrix4fv(
                glGetUniformLocation(program, name.c_str()), 1,  GL_FALSE, (GLfloat*)value_ptr(value));
        }
        else if constexpr(std::is_same_v<T, Vec3>) {
            glUniform3fv(
                glGetUniformLocation(program, name.c_str()), 1, (GLfloat*)value_ptr(value));
        }
        // TODO: add any other needed uniform types
    }

private:
    GLuint program;
};



#endif //SHADER_HPP
