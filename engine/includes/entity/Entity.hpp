#pragma once

#include "../main.hpp"
#include "../components/ComponentBase.hpp"

class Entity {
    public:
        int id_;
        bool is_dead_ = false;;
        std::vector<std::shared_ptr<ComponentBase>> components_;
        Entity(int arg_id, std::vector<std::shared_ptr<ComponentBase>> arg_components);

        Entity(const Entity& other);

        int GetId();

        void SetId(int arg_id);

        std::shared_ptr<Entity> Clone() const;

        bool HasComponent(const std::type_info& arg_type) const;

        template <class ComponentType>
        std::shared_ptr<ComponentType> GetComponent() const {
            for (const auto& component : components_) {
                auto casted_component = std::dynamic_pointer_cast<ComponentType>(component);
                if (casted_component) {
                    return casted_component;
                }
            }
            return nullptr;
        }
};