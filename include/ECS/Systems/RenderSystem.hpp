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
};



#endif //RENDERSYSTEM_HPP
