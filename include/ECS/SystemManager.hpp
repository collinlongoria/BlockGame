//
// Created by colli on 12/26/2024.
//

#ifndef SYSTEMMANAGER_HPP
#define SYSTEMMANAGER_HPP
#include <memory>

#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include "Core/Log.hpp"

class SystemManager {
public:
  template<typename T>
  std::shared_ptr<T> RegisterSystem() {
    const char* type = typeid(T).name();

    if(systems.find(type) != systems.end()) {
      Log::Output(Log::Severity::ERROR, "Registering system more than once.");
      return nullptr;
    }

    // Create pointer to the system and return it
    auto system = std::make_shared<T>();
    systems.insert({type, system});
    return system;
  }

  template<typename T>
  void SetSignature(Signature signature) {
    const char* type = typeid(T).name();

    if(systems.find(type) == systems.end()) {
      Log::Output(Log::Severity::ERROR, "System used before being registered..");
      return;
    }

    // Set the signature for system
    signatures.insert({type, signature});
  }

  void EntityDestroyed(Entity entity) {
    // Erase destroyed entity from all system lists
    for(auto const& pair : systems) {
      auto const& system = pair.second;

      system->entities.erase(entity);
    }
  }

  void EntitySignatureChanged(Entity entity, Signature signature) {
    // notify each system of signature change
    for(auto const& pair : systems) {
      auto const& type = pair.first;
      auto const& system = pair.second;
      auto const& sysSignature = signatures[type];

      // Entity signature matches system: insert into set
      if((signature & sysSignature) == sysSignature) {
        system->entities.insert(entity);
      }
      // Entity does not match: erase from the set
      else {
        system->entities.erase(entity);
      }
    }
  }

private:
  // Map from system type string pointer to a signature
  std::unordered_map<const char*, Signature> signatures{};

  // Map from system type string pointer to a system pointer
  std::unordered_map<const char*, std::shared_ptr<System>> systems{};
};



#endif //SYSTEMMANAGER_HPP
