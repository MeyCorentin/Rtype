#pragma once

#include "../main.hpp"
#include "ComponentBase.hpp"

template <class T>
class Component: public ComponentBase {
    private:
        T value_;
        const std::type_info& original_type_;

    public:
        Component(T arg_n) :
            value_(arg_n), original_type_(typeid(*this)) {}

        Component() :
            value_(), original_type_(typeid(*this)) {}

        const std::type_info& GetType() const override {
            return original_type_;
        }

        T &getValue() {
            return value_;
        }
        void setValue(T arg_new_value) {
            value_ = arg_new_value;
        };
        std::shared_ptr<ComponentBase> Clone() const override {
            return std::make_shared<Component>(*this);
        }
};