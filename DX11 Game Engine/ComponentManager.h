#pragma once

#include <unordered_map>
#include <memory>

#include "Types.h"
#include "ComponentList.h"

class ComponentManager {

public:

    template<typename T> void registerComponent() {
        const char* typeName = typeid(T).name();
        assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once.");
        componentTypes.insert({ typeName, nextComponentType });
        componentLists.insert({ typeName, std::make_shared<ComponentList<T>>() });
        nextComponentType++;
    }

    template<typename T> ComponentType getComponentType() {
        const char* typeName = typeid(T).name();
        assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");
        return componentTypes[typeName];
    }

    template<typename T> void addComponent(EntityID entity, T component) {
        getComponentList<T>()->insert(entity, component);
    }

    template<typename T> void removeComponent(EntityID entity) {
        getComponentList<T>()->remove(entity);
    }

    template<typename T> T& getComponent(EntityID entity) {
        return getComponentList<T>()->get(entity);
    }

    void entityDestroyed(EntityID entity) {
        for (auto const& pair : componentLists) {
            auto const& component = pair.second;
            component->entityDestroyed(entity);
        }
    }

private:
    // Map from type string pointer to a component type
    std::unordered_map<const char*, ComponentType> componentTypes{};

    // Map from type string pointer to a component array
    std::unordered_map<const char*, std::shared_ptr<ComponentListInterface>> componentLists{};

    // The component type to be assigned to the next registered component - starting at 0
    ComponentType nextComponentType{};

    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
    template<typename T> std::shared_ptr<ComponentList<T>> getComponentList() {
        const char* typeName = typeid(T).name();
        assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");
        return std::static_pointer_cast<ComponentList<T>>(componentLists[typeName]);
    }

};