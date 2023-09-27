#pragma once

#include "../main.hpp"
#include "../entity/EntityBuilder.hpp"
#include "../system/TSystem.hpp"

class TScene {
    private:
        std::vector<std::shared_ptr<TEntity>> entities;
        std::vector<std::shared_ptr<TSystem>> systems;
        std::vector<std::shared_ptr<sf::Sprite>> sprites;
        std::vector<std::shared_ptr<sf::Texture>> textures;

        std::unordered_map<sf::Keyboard::Key, bool> keyStates;
        bool running = true;
        double targetFrameTime = (1.0 / 60);

    public:
        TScene( std::vector<std::shared_ptr<TSystem>> _systemList,
                std::vector<std::shared_ptr<TEntity>> _entityList,
                std::vector<std::shared_ptr<sf::Sprite>> _spriteList,
                std::vector<std::shared_ptr<sf::Texture>> _textureList)
            : systems(_systemList), entities(_entityList), sprites(_spriteList), textures(_textureList) {
                for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
                    keyStates[static_cast<sf::Keyboard::Key>(i)] = false;
                }
            }

        std::vector<std::shared_ptr<TEntity>> getEntities()
        {
            return  entities;
        }
        std::vector<std::shared_ptr<TSystem>> getSystems()
        {
            return systems;
        }

        void run() {
            sf::RenderWindow window(sf::VideoMode(1000, 1000), "R-Type");
            sf::Event event;
            std::vector<int> inputs = {0, 0, 0, 0};
            std::cout << "[LAUNCH] Scene run" << std::endl;
            while (running)
            {
                window.clear();
                inputs = {0, 0, 0, 0};
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    if (event.type == sf::Event::KeyPressed)
                        keyStates[event.key.code] = true;
                    if (event.type == sf::Event::KeyReleased)
                        keyStates[event.key.code] = false;
                }
                auto startTime = std::chrono::high_resolution_clock::now();
                inputs[0] = keyStates[sf::Keyboard::Z];
                inputs[1] = keyStates[sf::Keyboard::Q];
                inputs[2] = keyStates[sf::Keyboard::S];
                inputs[3] = keyStates[sf::Keyboard::D];
                for (const auto& system : systems)
                    system->compute(entities, window, inputs);
                auto endTime =  std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
                if (elapsedTime < targetFrameTime) {
                    auto sleepTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(targetFrameTime - elapsedTime));
                    std::this_thread::sleep_for(sleepTime);
                }
                window.display();
            }
        }
};
