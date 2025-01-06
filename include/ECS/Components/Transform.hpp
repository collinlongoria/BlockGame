//
// Created by colli on 12/26/2024.
//

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Core/Math.hpp"

struct Transform
{
    // Current position
    Vec3 position;

    // Current rotation
    Vec3 rotation;

    // Current scale
    Vec3 scale;

    // Current matrix
    Mat4 matrix;

    // Last known values
    // Used by Transform System for updating
    Vec3 old_position, old_rotation, old_scale;
};

#endif //TRANSFORM_HPP
