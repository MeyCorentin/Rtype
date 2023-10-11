#pragma once

#include "../main.hpp"
#include "../entity/EntityBuilder.hpp"
#include <list>
#include "../system/System.hpp"

class Scene {
    private:
        std::shared_ptr<sf::RenderWindow> window_;
        std::shared_ptr<sf::Event> event_;
        std::vector<std::shared_ptr<Entity>> list_entities_;
        std::vector<std::shared_ptr<Entity>> entities_;
        std::vector<std::shared_ptr<System>> systems_;
        std::vector<std::shared_ptr<sf::Sprite>> sprites_;
        std::vector<std::shared_ptr<sf::Texture>> textures_;
        std::vector<std::pair<int, std::vector<int>>> spawn_index_;
        int frames_this_second_;
        int total_ticks_;
        std::unordered_map<sf::Keyboard::Key, int> key_states_;
        std::shared_ptr<sf::Clock> second_clock_;
        std::shared_ptr<sf::Text> tick_;
        std::shared_ptr<sf::Text> entities_nbr_;
        std::shared_ptr<sf::Font> font_;
        std::shared_ptr<sf::Text> current_tick_;

        int time_pressed_;
        bool is_pressed_ = false;
        std::vector<int> inputs_ = {0, 0, 0, 0, 0};

    public:
        Scene(){}
        bool running = true;
        double targetFrameTime = (1.0 / 60);
        Scene( std::vector<std::shared_ptr<System>> arg_system_list,
                std::vector<std::shared_ptr<Entity>> arg_entity_list,
                std::vector<std::shared_ptr<sf::Sprite>> arg_sprite_list,
                std::vector<std::shared_ptr<sf::Texture>> arg_texture_list,
                std::vector<std::pair<int, std::vector<int>>> arg_spawn_index)
            : systems_(arg_system_list), list_entities_(arg_entity_list), sprites_(arg_sprite_list), textures_(arg_texture_list), spawn_index_(arg_spawn_index)  {
                window_ = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(1000, 1000), "R-Type"));
                frames_this_second_ = 0;
                total_ticks_ = 0;
                event_ = std::shared_ptr<sf::Event>(new sf::Event());
                second_clock_ = std::shared_ptr<sf::Clock>(new sf::Clock());
                entities_nbr_ = std::shared_ptr<sf::Text>(new sf::Text());
                font_ = std::shared_ptr<sf::Font>(new sf::Font());
                font_->loadFromFile("../../rtype/sources/fonts/arial.ttf");
                tick_ = std::shared_ptr<sf::Text>(new sf::Text());
                tick_->setPosition(0, 0);
                tick_->setFillColor(sf::Color::White);
                tick_->setCharacterSize(14),
                tick_->setFont(*font_);

                entities_nbr_->setPosition(0, 20);
                entities_nbr_->setFillColor(sf::Color::White);
                entities_nbr_->setCharacterSize(14),
                entities_nbr_->setFont(*font_);


                current_tick_ = std::shared_ptr<sf::Text>(new sf::Text());
                current_tick_->setPosition(0, 40);
                current_tick_->setFillColor(sf::Color::White);
                current_tick_->setCharacterSize(14),
                current_tick_->setFont(*font_);

                for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
                    key_states_[static_cast<sf::Keyboard::Key>(i)] = false;
                }
            }


        void ClearWindow()
        {
            window_->clear();
        }

        std::vector<std::shared_ptr<Entity>> GetEntities()
        {
            return  entities_;
        }

        std::vector<std::shared_ptr<System>> GetSystems()
        {
            return systems_;
        }

        void DisplayCurrentTick() {
            int &ref = total_ticks_;
            std::string tick_string;

            ref += 1;
            tick_string = std::to_string(ref) + " (ticks)";
            current_tick_->setString(tick_string);
            window_->draw(*current_tick_);
        }

        void DisplayEntities(int nbr) {
            std::string nbr_string = std::to_string(nbr) + " (enttities)";

            this->entities_nbr_->setString(nbr_string);
            window_->draw(*entities_nbr_);
        }

        void DisplayTicks() {
            int &ref = frames_this_second_;
            std::string tick_string;

            ref += 1;
            if (second_clock_->getElapsedTime().asSeconds() >= 1.0) {
                tick_string = std::to_string(ref) + " (t/s)";
                tick_->setString(tick_string);
                ref= 0;
                second_clock_->restart();
            }
            window_->draw(*tick_);
        }

        void Update()
        {
            this->ClearWindow();

            for (const auto& system : systems_)
                system->Compute(entities_, window_, inputs_, sprites_, textures_, event_);

            for (const auto& spawn_info : spawn_index_)
            {
                if (spawn_info.first != total_ticks_)
                    continue;
                for (int value : spawn_info.second)
                {
                    for (const auto& entity : list_entities_){
                        if (value != entity.get()->GetId())
                            continue;
                        std::cout << "Spawn" << std::endl;
                        entities_.push_back(std::make_shared<Entity>(*entity));
                    }
                }
            }
            DisplayTicks();
            DisplayEntities(entities_.size());
            DisplayCurrentTick();
            auto it = entities_.begin();
            while (it != entities_.end())
                ((*it)->is_dead_) ? it = entities_.erase(it) : ++it;
            window_->display();
        }
};
