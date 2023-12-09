#pragma once

#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "../graphics/sprite/sprite.cpp"
#include "../language/string.cpp"
#include "storyboard_utils.cpp"

class Storyboard {
public:
  std::string root;
  std::vector<std::shared_ptr<Sprite>> sprites;
  AssetManager assets;

  // FNs
  void parse_file(std::string path);
  void parse_lines(std::vector<std::string> lines);

  void load_sprite(std::string header, std::vector<std::string> commands);

  void update(float time);
  void draw(float time);
};

void Storyboard::parse_file(std::string path) {
  std::fstream storyboard_file;
  storyboard_file.open(path, std::ios::in);

  if (storyboard_file.is_open()) {
    std::vector<std::string> lines;

    std::string buffer;
    while (std::getline(storyboard_file, buffer)) {
      lines.push_back(buffer);
    }

    storyboard_file.close();

    parse_lines(lines);
  } else {
    std::printf("[Storyboard] Failed to read storyboard file! \n");
  }

  std::printf("[Storyboard] Loaded %zu sprites!", sprites.size());
}

void Storyboard::parse_lines(std::vector<std::string> lines) {
  std::printf("[Storyboard] Amount of lines %zu \n", lines.size());

  std::string current_section = "";
  std::string current_sprite = "";
  std::string current_layer = "";
  std::vector<std::string> commands;
  std::map<std::string, std::string> variables;

  for (std::string line : lines) {
    std::string line_copy = line;
    StringUtils::trim(line_copy);

    if (line.rfind("//", 0) == 0) {
      current_layer = std::strtok(&line[0], "//");
      continue;
    }

    if (line_copy.size() == 0) {
      continue;
    }

    auto section = StoryboardUtils::parse_header(line_copy);

    if (section.size() != 0) {
      current_section = section;
      continue;
    }

    // No switch case for strings, OK
    if (current_section == "Variables") {
      auto items = StoryboardUtils::parse_variables(line_copy);
      variables[items[0]] = items[1];
    } else if (current_section == "Events") {
      // Variables
      if (line_copy.find("$") != std::string::npos) {
        for (const auto &element : variables) {
          if (line_copy.find(element.first) != std::string::npos) {
            line_copy =
                line_copy.replace(line_copy.find(element.first),
                                  sizeof(element.first), element.second);
          }
        }
      }

      // Sprite
      if ((line_copy.rfind("Sprite", 0) == 0) ||
          (line_copy.rfind("Animation", 0) == 0)) {
        if (current_sprite.size() != 0) {
          // std::printf("[Storyboard] Load, %zu commands! \n",
          // commands.size());
          load_sprite(current_sprite, commands);
        }

        current_sprite = line_copy;
        commands.clear();
      } else if ((line.rfind(" ", 0) == 0) || (line.rfind("_", 0) == 0)) {
        commands.push_back(line);
      }
    }
  }

  // Somehow missed it
  if (current_sprite.size() > 0) {
    load_sprite(current_sprite, commands);
    commands.clear();
  }
}

void Storyboard::load_sprite(std::string header,
                             std::vector<std::string> commands) {
  auto items = StoryboardUtils::parse_commas(header);

  std::string filename = items[3]
                             .replace(items[3].find('"'), sizeof('"'), "")
                             .replace(items[3].find('"'), sizeof('"'), "");

  std::string texture_path =
      "/run/media/junko/4th/Projects/TypeT/assets/Future Candy/" + filename;

  for (int i = 0; i < 25; i++) {
    try {
      texture_path =
          texture_path.replace(texture_path.find('\\'), sizeof('\\'), "/");
    } catch (...) {
      continue;
    }
  }

  StringUtils::trim(texture_path);

  if (!std::filesystem::exists(texture_path)) {
    std::printf("[Sprite] Texture not found: %s \n", texture_path.c_str());
    exit(1);
  }

  if (items[0] == "Sprite") {
    Math::Vector2<float> position;
    position.x = std::atof(items[4].c_str());
    position.y = std::atof(items[5].c_str());

    Math::OriginType origin = Math::get_origin_from_str(items[2]);

    std::shared_ptr<Sprite> current_sprite = std::make_shared<Sprite>();
    current_sprite->textures.push_back(assets.load_image(texture_path));

    current_sprite->origin = origin;
    current_sprite->position.x = position.x;
    current_sprite->position.y = position.y;

    current_sprite->size.x = current_sprite->textures[0]->size.width;
    current_sprite->size.y = current_sprite->textures[0]->size.height;

    current_sprite->transforms =
        StoryboardUtils::parse_sprite_commands(commands);

    if (current_sprite->transforms.size() == 0) {
      return;
    }

    float min_time = current_sprite->transforms[0]->time.start;
    float max_time =
        current_sprite->transforms[current_sprite->transforms.size() - 1]
            ->time.end;

    for (auto const &transform : current_sprite->transforms) {
      min_time = std::min<float>(min_time, transform->time.start);
      max_time = std::max<float>(max_time, transform->time.end);
    }

    current_sprite->time.start = min_time;
    current_sprite->time.end = max_time;
    current_sprite->reset_to_transforms();

    // std::printf("TIME: %f %f \n", min_time, max_time);

    sprites.push_back(current_sprite);
  }
}

// Update
void Storyboard::update(float time) {
  auto it = sprites.begin();
  while (it != sprites.end()) {
    auto const &sprite = *it;
    sprite->update(time);

    if (time > sprite->time.end) {
      std::printf("[Storyboard] Removed sprites, %zu left! \n", sprites.size());
      it = sprites.erase(it);
    } else {
      ++it;
    }
  }

  assets.update();
}
// Draw
void Storyboard::draw(float time) {
  update(time);

  for (int i = 0; i < sprites.size(); i++) {
    if (time >= sprites[i]->time.start &&
        time <= sprites[i]->time.end + 50.0f) {
      sprites[i]->draw();
    }
  }
}