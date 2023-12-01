#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../graphics/sprite/sprite.cpp"

class Storyboard {
public:
  std::string root;
  std::vector<Sprite> sprites;
};