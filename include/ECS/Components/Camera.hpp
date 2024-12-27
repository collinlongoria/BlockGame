//
// Created by colli on 12/26/2024.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Core/Math.hpp"

struct Camera {
  Mat4 projectionMatrix;

  static Mat4 ProjectionMatrix(float fov, float nearClip, float farClip, unsigned int viewWidth, unsigned int viewHeight) {
    // calculate aspect ratio
    float aspectRatio = static_cast<float>(viewWidth) / static_cast<float>(viewHeight);

    // create matrix
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
  }
};

#endif //CAMERA_HPP
