//
// Created by colli on 12/26/2024.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Core/Math.hpp"
#include <bitset>

// Component type alias
using ComponentType = std::uint8_t;

// Max number of components
const ComponentType MAX_COMPONENTS = 32;

// Bitset of component type
using Signature = std::bitset<MAX_COMPONENTS>;

#endif //COMPONENT_HPP
