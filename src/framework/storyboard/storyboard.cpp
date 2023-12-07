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
  std::vector<Sprite> sprites;

  // FNs
  void parse_file(std::string path);
  void parse_lines(std::vector<std::string> lines);
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
          std::printf("[Storyboard] Load, %zu commands! \n", commands.size());
        }

        current_sprite = line_copy;
        commands.clear();
      } else if ((line.rfind(" ", 0) == 0) || (line.rfind("_", 0) == 0)) {
        commands.push_back(line);
      }
    }
  }
}
