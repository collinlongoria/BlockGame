//
// Created by colli on 12/26/2024.
//

#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include "ECS/System.hpp"

class PhysicsSystem : public System {
public:
    void Init();

    void Update(float dt);
};

#endif //PHYSICSSYSTEM_HPP
