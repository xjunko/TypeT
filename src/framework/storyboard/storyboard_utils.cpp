#pragma once

#include <string>
#include <vector>

#include "../language/string.cpp"

namespace StoryboardUtils {

static inline std::string parse_header(std::string s) {
  if (s.rfind("[", 0) == 0) {
    auto ss = s.replace(s.find("["), sizeof("[") - 1, "")
                  .replace(s.find("]"), sizeof("]") - 1, "");

    StringUtils::trim(ss);

    return ss;
  }

  return "";
}

static inline std::vector<std::string> parse_variables(std::string s) {
  auto ss = s;
  StringUtils::trim(ss);
  return StringUtils::split(ss, "=");
}

static inline std::vector<std::string> parse_commas(std::string s) {
  auto ss = s;
  StringUtils::trim(ss);
  return StringUtils::split(ss, ",");
}

// Parse Helpers
std::shared_ptr<Transform::Transform>
parse_command(std::vector<std::string> items) {
  if (items[0] == "T" || items[0] == "L") {
    std::printf(
        "[Panic] Loop commands in normal parser!!! This should not happen!!");
    exit(1);
  }

  std::string command_type = items[0];

  float start_time = std::atof(items[2].c_str());

  if (items[3] == "") {
    items[3] = items[2];
  }

  float end_time = std::max<float>(std::atof(items[3].c_str()), start_time);
  int argument = 0;

  // HACK: bruh
  if (end_time == start_time) {
    end_time = start_time + 1.0f;
  }

  if (command_type == "F" || command_type == "R" || command_type == "S" ||
      command_type == "MX" || command_type == "MY") {
    argument = 1;
  } else if (command_type == "M" || command_type == "V") {
    argument = 2;
  } else if (command_type == "C") {
    argument = 3;
  }

  // Transform
  std::shared_ptr<Transform::Transform> current_transform =
      std::make_shared<Transform::Transform>();

  current_transform->type = Transform::Type::_INVALID;
  current_transform->time.start = start_time;
  current_transform->time.end = end_time;

  if (argument == 0) {
    if (command_type == "A") {
      current_transform->type = Transform::Type::ADDITIVE;
    } else if (command_type == "V") {
      current_transform->type = Transform::Type::FLIP_VERT;
    } else if (command_type == "H") {
      current_transform->type = Transform::Type::FLIP_HORIZ;
    }

    return current_transform;
  }

  // Sections parsing
  std::vector<std::string> parameters(items.begin() + 4, items.end());
  std::vector<std::vector<float>> sections;
  int sections_length = parameters.size() / argument;

  for (int i = 0; i < sections_length; i++) {
    std::vector<float> current_section;
    sections.push_back(current_section);

    for (int j = 0; j < argument; j++) {
      sections[i].push_back(std::atof(parameters[argument * i + j].c_str()));

      if (command_type[0] == 'F') {
        sections[i][j] *= 255;
      }
    }
  }

  if (sections.size() == 1) {
    sections.push_back(sections[0]);
  }

  // Multiple arguments and type
  current_transform->easing = Easing::linear;
  current_transform->before = sections[0];
  current_transform->after = sections[1];

  std::map<std::string, Transform::Type> transform_mapping = {
      {"F", Transform::Type::FADE},
      {"R", Transform::Type::ANGLE},
      {"S", Transform::Type::SCALE_MULTIPLY_BY},
      {"MX", Transform::Type::MOVE_X},
      {"MY", Transform::Type::MOVE_Y},
      {"M", Transform::Type::MOVE},
      {"V", Transform::Type::SCALE_VECTOR},
      {"C", Transform::Type::COLOR},
  };

  for (const auto &element : transform_mapping) {
    if (command_type == element.first) {
      current_transform->type = element.second;
    }
  }

  return current_transform;
}

std::vector<std::shared_ptr<Transform::Transform>>
parse_sprite_commands(std::vector<std::string> commands) {
  std::vector<std::shared_ptr<Transform::Transform>> transforms;

  for (std::string subcommand : commands) {
    std::vector<std::string> command =
        StoryboardUtils::parse_commas(subcommand);

    if (command[0] != "L") {
      auto transform = parse_command(command);

      if (transform->type == Transform::Type::_INVALID) {
        continue;
      }
      transforms.push_back(transform);
    }
  }

  return transforms;
}

} // namespace StoryboardUtils