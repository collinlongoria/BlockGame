//
// Created by colli on 12/26/2024.
//

#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <set>

#include "Entity.hpp"

class System {
public:
    std::set<Entity> entities;
};

#endif //SYSTEM_HPP
