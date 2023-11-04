#include "system/S_AutoMove.hpp"

std::vector<std::shared_ptr<Entity>> S_AutoMove::Filter(const std::vector<std::shared_ptr<Entity>>& arg_entities) {
    std::vector<std::shared_ptr<Entity>> filtered_entities;

    for (const std::shared_ptr<Entity>& entity : arg_entities) {
        if (entity->HasComponent(typeid(C_ClockAutoMove<sf::Clock>)) &&
            entity->HasComponent(typeid(C_TimeAutoMove<int>)) &&
            entity->HasComponent(typeid(C_IsAutoMove<bool>))) {
            filtered_entities.push_back(entity);
        }
    }
    return filtered_entities;
}

void S_AutoMove::Execute(
    int arg_is_server,
    std::vector<std::shared_ptr<Entity>>& arg_entities,
    std::shared_ptr<sf::RenderWindow> arg_window,
    std::vector<int> arg_inputs,
    std::vector<std::shared_ptr<Entity>>& arg_all_entities,
    std::vector<std::shared_ptr<sf::Music>>& arg_music_list,
    std::shared_ptr<sf::Event> event_) {
        std::shared_ptr<C_IsAutoMove<bool>> is_auto_move;
        std::shared_ptr<C_ClockAutoMove<sf::Clock>> clock;
        std::shared_ptr<C_TimeAutoMove<int>> time_auto_move;
        for (const std::shared_ptr<Entity>& entity : arg_entities) {
            is_auto_move = entity->template GetComponent<C_IsAutoMove<bool>>();
            clock = entity->template GetComponent<C_ClockAutoMove<sf::Clock>>();
            time_auto_move = entity->template GetComponent<C_TimeAutoMove<int>>();

            if (is_auto_move->getValue()) {
                if (clock->getValue().getElapsedTime().asSeconds() > time_auto_move->getValue())
                    is_auto_move->getValue() = false;
            }
        }
    }