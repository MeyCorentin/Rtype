#include "system/S_Input.hpp"

std::vector<std::shared_ptr<Entity>> S_Input::Filter(const std::vector<std::shared_ptr<Entity>>& arg_entities) {
    std::vector<std::shared_ptr<Entity>> filtered_entities;

    for (const std::shared_ptr<Entity>& entity : arg_entities) {
        if (entity->HasComponent(typeid(C_Player<int>)) &&
            entity->HasComponent(typeid(C_Position<std::pair<double, double>>)) &&
            entity->HasComponent(typeid(C_IsMoving<bool>)) &&
            entity->HasComponent(typeid(C_Admin<bool>)) &&
            entity->HasComponent(typeid(C_Weapon<std::pair<int, int>>)) &&
            entity->HasComponent(typeid(C_ChargedShoot<sf::Clock>)) &&
            entity->HasComponent(typeid(C_PlayerMovementClock<sf::Clock>)) 
            ) {
            filtered_entities.push_back(entity);
        }
    }
    return filtered_entities;
}

void DrawEntityID(
    std::shared_ptr<sf::RenderWindow> arg_window,
    const std::shared_ptr<Entity>& entity,
    sf::Font arg_font,
    sf::Text arg_entity_id) {
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp = entity->template GetComponent<C_Position<std::pair<double, double>>>();
    if (position_comp)
    {
        arg_entity_id.setPosition(position_comp->getValue().first, position_comp->getValue().second);
        arg_entity_id.setString(std::to_string(entity->GetId()));
        arg_window->draw(arg_entity_id);
    }
}


void DrawEntityHitbox(
    std::shared_ptr<sf::RenderWindow> arg_window,
    const std::shared_ptr<Entity>& entity,
    sf::Color outlineColor) {
    std::shared_ptr<C_Hitbox<std::pair<int, int>>> hitbox_comp = entity->template GetComponent<C_Hitbox<std::pair<int, int>>>();
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp = entity->template GetComponent<C_Position<std::pair<double, double>>>();
    sf::RectangleShape hitbox;
    hitbox.setPosition(position_comp->getValue().first, position_comp->getValue().second);
    hitbox.setSize(sf::Vector2f(hitbox_comp->getValue().first, hitbox_comp->getValue().second));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(outlineColor);
    hitbox.setOutlineThickness(1.0f);
    arg_window->draw(hitbox);
}


void S_Input::Move(
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp, 
    std::shared_ptr<sf::RenderWindow> arg_window,
    const std::shared_ptr<Entity>& entity,
    std::vector<std::shared_ptr<Entity>>& arg_entities,
    sf::Font arg_font,
    sf::Text arg_entity_id,
    sf::Time elapsed)
{
    std::shared_ptr<C_Hitbox<std::pair<int, int>>> hitbox_size = entity->template GetComponent<C_Hitbox<std::pair<int, int>>>();

    int movement_elapsed = 5 * elapsed.asMilliseconds() / 30;
    if (inputs_[0] == 1)
    {
        if (position_comp->getValue().second > movement_elapsed)
            position_comp->setValue(std::make_pair(position_comp->getValue().first, position_comp->getValue().second - movement_elapsed));
    }
    if (inputs_[1] == 1)
    {
        if (position_comp->getValue().first > movement_elapsed)
            position_comp->setValue(std::make_pair(position_comp->getValue().first - movement_elapsed, position_comp->getValue().second));
    }
    if (inputs_[2] == 1)
    {
        if (position_comp->getValue().second < arg_window->getSize().y - movement_elapsed - hitbox_size->getValue().second)
            position_comp->setValue(std::make_pair(position_comp->getValue().first, position_comp->getValue().second  + movement_elapsed));
    }
    if (inputs_[3] == 1)
    {
        if (position_comp->getValue().first < arg_window->getSize().x - movement_elapsed- hitbox_size->getValue().first)
            position_comp->setValue(std::make_pair(position_comp->getValue().first + movement_elapsed, position_comp->getValue().second));
    }
    if (inputs_[5] == 1)
    {
        for (const std::shared_ptr<Entity>& entity : arg_entities) {
            std::shared_ptr<C_Player<int>> is_player = entity->template GetComponent<C_Player<int>>();
            std::shared_ptr<C_PlayerAmmo<bool>> is_player_ammo = entity->template GetComponent<C_PlayerAmmo<bool>>();
            std::shared_ptr<C_Bonus<bool>> is_bonus =  entity->template GetComponent<C_Bonus<bool>>();
            std::shared_ptr<C_Hitbox<std::pair<int, int>>> hitbox_comp = entity->template GetComponent<C_Hitbox<std::pair<int, int>>>();
            std::shared_ptr<C_Position<std::pair<double, double>>> position_comp = entity->template GetComponent<C_Position<std::pair<double, double>>>();
            sf::Color outlineColor;
            if (!hitbox_comp || !position_comp)
                continue;
            if (is_player || is_player_ammo) {
                outlineColor = sf::Color::Green;
            }
            else if (is_bonus) {
                outlineColor = sf::Color::Blue;
            }
            else {
                outlineColor = sf::Color::Red;
            }
            DrawEntityHitbox(arg_window, entity, outlineColor);
            DrawEntityID(arg_window, entity, arg_font, arg_entity_id);
        }

    }
}

void S_Input::ChangeAdminMode(
    const std::shared_ptr<Entity>& entity,
    std::shared_ptr<sf::Event> event_)
{
    std::shared_ptr<C_Life<int>> life = entity->template GetComponent<C_Life<int>>();
    std::shared_ptr<C_Admin<bool>> is_admin = entity->template GetComponent<C_Admin<bool>>();

    if (event_->key.code == sf::Keyboard::A) {
        if (is_admin->getValue()) {
            life->getValue() = 1;
            is_admin->getValue() = false;
        } else {
            life->getValue() = -1;
            is_admin->getValue() = true;
        }
    }
}

void S_Input::BasicShot(
    const std::shared_ptr<Entity>& entity,
    std::vector<std::shared_ptr<Entity>>& arg_all_entities,
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp,
    Scene * arg_scene)
{
    std::shared_ptr<C_ChargedShoot<sf::Clock>> clock = entity->template GetComponent<C_ChargedShoot<sf::Clock>>();

    if (clock->getValue().getElapsedTime().asSeconds() > 1)
        arg_scene->createEntity(arg_all_entities, 9, position_comp);
    else if (clock->getValue().getElapsedTime().asSeconds() > 0.6)
        arg_scene->createEntity(arg_all_entities, 8, position_comp);
    else if (clock->getValue().getElapsedTime().asSeconds() > 0.4)
        arg_scene->createEntity(arg_all_entities, 7, position_comp);
    else if (clock->getValue().getElapsedTime().asSeconds() > 0.2)
        arg_scene->createEntity(arg_all_entities, 6, position_comp);
    else if (clock->getValue().getElapsedTime().asSeconds() > 0.1)
        arg_scene->createEntity(arg_all_entities, 5, position_comp);
    else
        arg_scene->createEntity(arg_all_entities, 3, position_comp);
}

void S_Input::SpecialShot(
    const std::shared_ptr<Entity>& entity,
    std::vector<std::shared_ptr<Entity>>& arg_all_entities,
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp,
    Scene * arg_scene)
{
    std::shared_ptr<C_Inventory<std::vector<std::shared_ptr<Entity>>>> vector_entities = entity->template GetComponent<C_Inventory<std::vector<std::shared_ptr<Entity>>>>();

    //Player attacks
    std::shared_ptr<C_Weapon<std::pair<int, int>>> weapon_player = entity->template GetComponent<C_Weapon<std::pair<int, int>>>();

    std::list<int> my_list_1 = {22, 23, 24};
    std::list<int> my_list_2 = {38, 39, 40};
    std::list<int> my_list_3 = {4, 29};

    for (std::shared_ptr<Entity>& v_entity: vector_entities->getValue()) {
        if ((std::find(my_list_1.begin(), my_list_1.end(), v_entity->GetBaseId()) != my_list_1.end())) {
            std::shared_ptr<C_Weapon<std::pair<int, int>>> weapon = v_entity->template GetComponent<C_Weapon<std::pair<int, int>>>();
            std::shared_ptr<C_Position<std::pair<double, double>>> position_force = v_entity->template GetComponent<C_Position<std::pair<double, double>>>();
            if (weapon->getValue().first != -1) {
                if (weapon->getValue().first == 25) {
                    for (int i = 0; i < 1; i++) { //TODO Set to 5
                        arg_scene->createEntity(arg_all_entities, weapon->getValue().first, position_force);
                        arg_scene->createEntity(arg_all_entities, 32, position_force); //TODO Change that
                    }
                }
                if (weapon->getValue().first == 26) {
                    arg_scene->createEntity(arg_all_entities, weapon->getValue().first, position_force);
                }
            }
        }
        if ((std::find(my_list_2.begin(), my_list_2.end(), v_entity->GetBaseId()) != my_list_2.end())) {
            std::shared_ptr<C_Weapon<std::pair<int, int>>> weapon = v_entity->template GetComponent<C_Weapon<std::pair<int, int>>>();
            std::shared_ptr<C_Position<std::pair<double, double>>> position_force = v_entity->template GetComponent<C_Position<std::pair<double, double>>>();
            if (weapon->getValue().second != -1) {
                if (weapon->getValue().second == 32) {
                    for (int i = 0; i < 1; i++) { //TODO Set to 5
                        arg_scene->createEntity(arg_all_entities, weapon->getValue().second, position_force);
                        arg_scene->createEntity(arg_all_entities, 25, position_force); //TODO Change that
                    }
                }
                if (weapon->getValue().second == 43) {
                    arg_scene->createEntity(arg_all_entities, weapon->getValue().second, position_force);
                }
            }
        }
    }

    //Drone attacks
    for (std::shared_ptr<Entity>& v_entity: vector_entities->getValue()) { //Check if vector_entities contain a drone left or right
        if (v_entity->GetBaseId() == 27)
            v_entity->is_dead_ = true;
        if ((std::find(my_list_3.begin(), my_list_3.end(), v_entity->GetBaseId()) != my_list_3.end()) && weapon_player->getValue().first == 26) {
            std::shared_ptr<C_Position<std::pair<double, double>>> position_drone = v_entity->template GetComponent<C_Position<std::pair<double, double>>>();
            std::shared_ptr<C_Weapon<std::pair<int, int>>> weapon = v_entity->template GetComponent<C_Weapon<std::pair<int, int>>>();
            for (std::shared_ptr<Entity>& v_entity: vector_entities->getValue()) {
                if ((std::find(my_list_1.begin(), my_list_1.end(), v_entity->GetBaseId()) != my_list_1.end())) {
                    arg_scene->createEntity(arg_all_entities, weapon->getValue().first, position_drone);
                }
            }
            for (std::shared_ptr<Entity>& v_entity: vector_entities->getValue()) {
                if ((std::find(my_list_2.begin(), my_list_2.end(), v_entity->GetBaseId()) != my_list_2.end())) {
                    arg_scene->createEntity(arg_all_entities, weapon->getValue().second, position_drone);
                }
            }
        }
    }
}

void S_Input::CheckTouchPressed(
    const std::shared_ptr<Entity>& entity,
    std::vector<std::shared_ptr<Entity>>& arg_all_entities,
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp,
    std::shared_ptr<sf::Event> event_,
    Scene * arg_scene)
{
    std::shared_ptr<C_SpriteRect<sf::IntRect>> rect = entity->template GetComponent<C_SpriteRect<sf::IntRect>>();
    std::shared_ptr<C_Sprite<sf::Sprite>> sprite = entity->template GetComponent<C_Sprite<sf::Sprite>>();
    std::shared_ptr<C_IsMoving<bool>> moving = entity->template GetComponent<C_IsMoving<bool>>();
    std::shared_ptr<C_Size<std::pair<std::pair<int, int>, std::pair<int, int>>>> size = entity->template GetComponent<C_Size<std::pair<std::pair<int, int>, std::pair<int, int>>>>();
    std::shared_ptr<C_ShootCharging<bool>> is_charging = entity->template GetComponent<C_ShootCharging<bool>>();
    std::shared_ptr<C_Inventory<std::vector<std::shared_ptr<Entity>>>> vector_entities = entity->template GetComponent<C_Inventory<std::vector<std::shared_ptr<Entity>>>>();
    std::shared_ptr<C_ChargedShoot<sf::Clock>> clock = entity->template GetComponent<C_ChargedShoot<sf::Clock>>();

    if (event_->type == sf::Event::KeyPressed) {
        if (event_->key.code == sf::Keyboard::Up) {
            inputs_[0] = 1;
            if (moving->getValue() == false) {
                moving->getValue() = true;
                rect->getValue().left = size->getValue().second.first + (rect->getValue().width * 2);
                sprite->getValue().setTextureRect(rect->getValue());
            }
        }
        if (event_->key.code == sf::Keyboard::Left)
            inputs_[1] = 1;
        if (event_->key.code == sf::Keyboard::Down) {
            inputs_[2] = 1;
            if (moving->getValue() == false) {
                moving->getValue() = true;
                rect->getValue().left = size->getValue().second.first - (rect->getValue().width * 2);
                sprite->getValue().setTextureRect(rect->getValue());
            }
        }
        if (event_->key.code == sf::Keyboard::Right)
            inputs_[3] = 1;
        if (event_->key.code == sf::Keyboard::Space) {
            if (is_charging->getValue() == false) {
                clock->getValue().restart();
                vector_entities->getValue().push_back(arg_scene->createEntity(arg_all_entities, 27, position_comp));
                is_charging->getValue() = true;
            }
        }
        if (event_->key.code == sf::Keyboard::Z && inputs_[5] == 1)
            inputs_[5] = 0;
        else if (event_->key.code == sf::Keyboard::Z && inputs_[5] == 0)
            inputs_[5] = 1;
    }
}

void S_Input::DropForce(
    const std::shared_ptr<Entity>& entity)
{
    std::shared_ptr<C_EntityMovementClock<sf::Clock>> entities_clock;
    std::shared_ptr<C_Inventory<std::vector<std::shared_ptr<Entity>>>> vector_entities = entity->template GetComponent<C_Inventory<std::vector<std::shared_ptr<Entity>>>>();

    std::list<int> my_list_1 = {22, 23, 24};
    std::list<int> my_list_2 = {38, 39, 40};

    for (std::shared_ptr<Entity>& v_entity: vector_entities->getValue()) {
        entities_clock = v_entity->template GetComponent<C_EntityMovementClock<sf::Clock>>();

        if ((std::find(my_list_1.begin(), my_list_1.end(), v_entity->GetBaseId()) != my_list_1.end()) || (std::find(my_list_2.begin(), my_list_2.end(), v_entity->GetBaseId()) != my_list_2.end())) {
            std::shared_ptr<C_ClockAutoMove<sf::Clock>> clock = v_entity->template GetComponent<C_ClockAutoMove<sf::Clock>>();
            std::shared_ptr<C_IsAutoMove<bool>> is_auto_move = v_entity->template GetComponent<C_IsAutoMove<bool>>();
            std::shared_ptr<C_Follow<bool>> is_follow = v_entity->template GetComponent<C_Follow<bool>>();

            if (is_follow->getValue()) {
                entities_clock->getValue().restart();
                is_follow->getValue() = false;
                is_auto_move->getValue() = true;
                clock->getValue().restart();
            }
        }
    }
}

void S_Input::CheckTouchReleased(
    const std::shared_ptr<Entity>& entity,
    std::vector<std::shared_ptr<Entity>>& arg_all_entities,
    std::shared_ptr<C_Position<std::pair<double, double>>> position_comp,
    std::shared_ptr<sf::Event> event_,
    Scene * arg_scene)
    {
    std::shared_ptr<C_ShootCharging<bool>> is_charging = entity->template GetComponent<C_ShootCharging<bool>>();
    std::shared_ptr<C_SpriteRect<sf::IntRect>> rect = entity->template GetComponent<C_SpriteRect<sf::IntRect>>();
    std::shared_ptr<C_Sprite<sf::Sprite>> sprite = entity->template GetComponent<C_Sprite<sf::Sprite>>();
    std::shared_ptr<C_IsMoving<bool>> moving = entity->template GetComponent<C_IsMoving<bool>>();
    std::shared_ptr<C_Size<std::pair<std::pair<int, int>, std::pair<int, int>>>> size = entity->template GetComponent<C_Size<std::pair<std::pair<int, int>, std::pair<int, int>>>>();
    std::shared_ptr<C_ChargedShoot<sf::Clock>> clock = entity->template GetComponent<C_ChargedShoot<sf::Clock>>();

    if (event_->type == sf::Event::KeyReleased) {
        if (event_->key.code == sf::Keyboard::Up) {
            inputs_[0] = 0;
            moving->getValue() = false;
            rect->getValue().left = size->getValue().second.first;
            sprite->getValue().setTextureRect(rect->getValue());
        }
        if (event_->key.code == sf::Keyboard::Num1) {
            arg_scene->need_switch_ = true;
            arg_scene->next_timeline_ = "1";
        }
        if (event_->key.code == sf::Keyboard::Num2) {
            arg_scene->need_switch_ = true;
            arg_scene->next_timeline_ = "2";
        }
        if (event_->key.code == sf::Keyboard::Num3) {
            arg_scene->need_switch_ = true;
            arg_scene->next_timeline_ = "3";
        }
        if (event_->key.code == sf::Keyboard::Num5) {
            arg_scene->need_switch_ = true;
            arg_scene->next_timeline_ = "5";
        }
        if (event_->key.code == sf::Keyboard::Num7) {
            arg_scene->need_switch_ = true;
            arg_scene->next_timeline_ = "7";
        }
        if (event_->key.code == sf::Keyboard::Left)
            inputs_[1] = 0;
        if (event_->key.code == sf::Keyboard::Down) {
            inputs_[2] = 0;
            moving->getValue() = false;
            rect->getValue().left = size->getValue().second.first;
            sprite->getValue().setTextureRect(rect->getValue());
        }
        if (event_->key.code == sf::Keyboard::Right)
            inputs_[3] = 0;
        if (event_->key.code == sf::Keyboard::Space) {
            BasicShot(entity, arg_all_entities, position_comp, arg_scene);
            SpecialShot(entity, arg_all_entities, position_comp, arg_scene);
            is_charging->getValue() = false;
        }
        if (event_->key.code == sf::Keyboard::D) {
            DropForce(entity);
        }
        ChangeAdminMode(entity, event_);
    }
}

void S_Input::Execute(
        int arg_is_server,
        Scene * arg_scene)  {
    std::vector<std::shared_ptr<Entity>> arg_entities =  Filter(arg_scene->entities_);
    sf::Font font_arg_;
    font_arg_.loadFromFile("../../rtype/sources/fonts/arial.ttf");
    sf::Text entity_id;
    std::shared_ptr<C_PlayerMovementClock<sf::Clock>> player_movement_clock;
    entity_id.setFillColor(sf::Color::Magenta);
    entity_id.setCharacterSize(10);
    entity_id.setFont(font_arg_);

    for (const std::shared_ptr<Entity>& entity : arg_entities) {
        std::shared_ptr<C_Position<std::pair<double, double>>> position_comp = entity->template GetComponent<C_Position<std::pair<double, double>>>();
        std::shared_ptr<C_Player<int>> player_id =  entity->template GetComponent<C_Player<int>>();
        player_movement_clock = entity->template GetComponent<C_PlayerMovementClock<sf::Clock>>();
        sf::Time elapsed = player_movement_clock->getValue().getElapsedTime();
        if (arg_is_server == 1)
            continue;
        if (!player_id)
            continue;
        if (&entity != &arg_entities.front())
            continue;
        while (arg_scene->window_->pollEvent(*arg_scene->event_)) {
            if (arg_scene->event_->type == sf::Event::Closed)
                arg_scene->window_->close();
            CheckTouchPressed(entity, arg_scene->entities_, position_comp, arg_scene->event_, arg_scene);
            CheckTouchReleased(entity, arg_scene->entities_, position_comp, arg_scene->event_, arg_scene);
        }
        Move(position_comp, arg_scene->window_, entity, arg_scene->entities_, font_arg_, entity_id, elapsed);
        player_movement_clock->getValue().restart();
    }
}