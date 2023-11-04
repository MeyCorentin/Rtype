#pragma once

#include "System.hpp"
#include "../components/C_Direction.hpp"
#include "../components/C_Speed.hpp"
#include "../components/C_Position.hpp"
#include "../components/C_IsAutoMove.hpp"

/**
 * @class S_Mouvement
 * @brief A class responsible for managing entity movement in the system.
 *
 * The S_Mouvement system is designed to filter entities with the necessary components for controlling
 * movement, such as direction, position, and speed. It provides functionality for updating the positions
 * of these entities, effectively managing their movement within the system.
 */
class S_Mouvement : public System {
    public:
    /**
     * @brief Filters a list of entities based on their components.
     *
     * Filters the input list of entities and returns a new list containing
     * entities with component(s):
     *      - C_Direction
     *      - C_Position
     *      - C_Speed
     *
     * @param arg_entities The list of entities to filter.
     * @return A filtered list of entities.
     */
    std::vector<std::shared_ptr<Entity>> Filter(const std::vector<std::shared_ptr<Entity>>& arg_entities) override;

    /**
     * @brief Handles movement for entities with movement-related components.
     *
     * This function processes entities with components related to movement, including direction, position,
     * and speed. It updates the positions of these entities based on their movement configurations, facilitating
     * their movement within the system.
     *
     * This function applies to entities that have the component(s):
     *      - C_Direction
     *      - C_Position
     *      - C_Speed
     *
     * @param arg_is_server An integer indicating whether the system is running on the server.
     * @param arg_entities A list of entities to apply the sinusoidal movement to.
     * @param arg_window A shared pointer to the rendering window.
     * @param arg_inputs A list of input values.
     * @param arg_all_entities A list of all entities in the system.
     * @param event_ A shared pointer to the event object.
     */
        void Execute(
                int arg_is_server,
                std::vector<std::shared_ptr<Entity>>& arg_entities,
                std::shared_ptr<sf::RenderWindow> arg_window,
                std::vector<int> arg_input,
                std::vector<std::shared_ptr<Entity>>& arg_all_Entities,
                std::vector<std::shared_ptr<sf::Music>>& arg_music_list,
                std::shared_ptr<sf::Event> event_) override;
};
