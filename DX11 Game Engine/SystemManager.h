#pragma once

#include <memory>
#include <unordered_map>

#include "Types.h"
#include "System.h"

class SystemManager {

public:
    template<typename T> std::shared_ptr<T> registerSystem() {
        const char* typeName = typeid(T).name();
        assert(systems.find(typeName) == systems.end() && "Registering system more than once.");
        auto system = std::make_shared<T>();
        systems.insert({ typeName, system });
        return system;
    }

    template<typename T> void setSignature(Signature signature) {
        const char* typeName = typeid(T).name();
        assert(systems.find(typeName) != systems.end() && "System used before registered.");
        signatures.insert({ typeName, signature });
    }

    void entityDestroyed(EntityID entity) {
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->entities.erase(entity);
        }
    }

    void entitySignatureChanged(EntityID entity, Signature entitySignature) {
        for (auto const& pair : systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            if ((entitySignature & systemSignature) == systemSignature) {
                system->entities.insert(entity);
            }
            else {
                system->entities.erase(entity);
            }
        }
    }

private:
    std::unordered_map<const char*, Signature> signatures{};
    std::unordered_map<const char*, std::shared_ptr<System>> systems{};

};