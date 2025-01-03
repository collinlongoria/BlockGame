//
// Created by colli on 12/26/2024.
//

#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include "ECS/System.hpp"
#include "Graphics/Shader.hpp"
#include <memory>

class RenderSystem : public System {
public:
    void Init();

    void Update(float dt);

private:
    std::unique_ptr<Shader> shader;

    Entity camera;

    GLuint VAO{}, VBO_V{}, VBO_N{};
};



#endif //RENDERSYSTEM_HPP
