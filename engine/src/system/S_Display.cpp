#include "system/S_Display.hpp"

std::vector<std::shared_ptr<Entity>> S_Display::Filter(const std::vector<std::shared_ptr<Entity>>& arg_entities) {
    std::vector<std::shared_ptr<Entity>> filtered_entities;

    for (const std::shared_ptr<Entity>& entity : arg_entities) {
        if (entity->HasComponent(typeid(C_Sprite<sf::Sprite>)) &&
            entity->HasComponent(typeid(C_Position<std::pair<double, double>>))) {
            filtered_entities.push_back(entity);
        }
    }
    return filtered_entities;
}

void S_Display::Execute(
        int arg_is_server,
        std::vector<std::shared_ptr<Entity>>& arg_entities,
        std::shared_ptr<sf::RenderWindow> arg_window,
        std::vector<int> arg_inputs,
        std::vector<std::shared_ptr<Entity>>& arg_all_entities,
        std::vector<std::shared_ptr<sf::Music>>& arg_music_list,
        std::shared_ptr<sf::Event> event_) {
    std::shared_ptr<C_Sprite<sf::Sprite>>sprite_comp;
    std::shared_ptr<C_Texture<sf::Texture>>texture_comp;
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp;
    std::shared_ptr<C_Background<bool>> is_background;
    float x_position;
    float y_position;
    for (const std::shared_ptr<Entity>& entity : arg_entities) {
        is_background = entity->template GetComponent<C_Background<bool>>();
        if (!is_background)
            continue;
        sprite_comp = entity->template GetComponent<C_Sprite<sf::Sprite>>();
        texture_comp = entity->template GetComponent<C_Texture<sf::Texture>>();
        if (texture_comp)
        {
            const sf::Texture* texture = sprite_comp->getValue().getTexture();
            if (texture == nullptr)
            {
                sprite_comp->getValue().setTexture(texture_comp->getValue());
            }
        }
        position_comp = entity->template GetComponent<C_Position<std::pair<double, double>>>();
        x_position = static_cast<float>(position_comp->getValue().first);
        y_position = static_cast<float>(position_comp->getValue().second);
        sprite_comp->getValue().setPosition(x_position, y_position);
        arg_window->draw(sprite_comp->getValue());
    }

    for (const std::shared_ptr<Entity>& entity : arg_entities) {
        is_background = entity->template GetComponent<C_Background<bool>>();
        if (is_background)
            continue;
        sprite_comp = entity->template GetComponent<C_Sprite<sf::Sprite>>();
        position_comp = entity->template GetComponent<C_Position<std::pair<double, double>>>();
        texture_comp = entity->template GetComponent<C_Texture<sf::Texture>>();
        if (texture_comp)
        {
<<<<<<< HEAD
            // std::cout << "(+)" << std::endl;
=======
>>>>>>> 47a3c0bad1318b59f097fe2ce39646459b71bd3a
            const sf::Texture* texture = sprite_comp->getValue().getTexture();
            if (texture == nullptr)
            {
                sprite_comp->getValue().setTexture(texture_comp->getValue());
            }
        }
        x_position = static_cast<float>(position_comp->getValue().first);
        y_position = static_cast<float>(position_comp->getValue().second);
        sprite_comp->getValue().setPosition(x_position, y_position);
        arg_window->draw(sprite_comp->getValue());
    }
}
