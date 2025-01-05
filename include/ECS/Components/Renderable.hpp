//
// Created by colli on 12/26/2024.
//

#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "Core/Math.hpp"

struct Renderable {
    uint32_t shaderID;
    uint32_t meshID;
    Vec3 color;
};

#endif //RENDERABLE_HPP
