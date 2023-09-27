#include <vector>
#include <cstdio>
#include <memory>
#include <typeinfo>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <map>
#include <nlohmann/json.hpp>
#include <functional>
#include <any>
#include <variant>

using Variant = std::variant<std::pair<std::shared_ptr<sf::Texture>, std::shared_ptr<sf::Sprite>>, int, std::pair<int, int>, bool>;
using json = nlohmann::json;