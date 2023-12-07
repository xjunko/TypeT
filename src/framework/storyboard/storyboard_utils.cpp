#pragma once

#include "../language/string.cpp"
#include <string>
#include <vector>

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
  int argument;

  // HACK: bruh
  if (end_time == start_time) {
    end_time = start_time + 1.0f;
  }

  switch (command_type[0]) {
  case 'F': {
    argument = 1;
    break;
  }

  case 'M': {
    argument = 2;
    break;
  }
  }

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

  if (sections_length == 1) {
    sections.push_back(sections[0]);
  }

  // Types
  if (command_type == "M") {
    std::shared_ptr<Transform::Transform> move_transform =
        std::make_shared<Transform::Transform>();

    move_transform->type = Transform::Type::MOVE;
    move_transform->easing = Easing::linear;
    move_transform->time = {.start = start_time, .end = end_time};
    move_transform->before = sections[0];
    move_transform->after = sections[1];

    return move_transform;
  } else if (command_type == "F") {
    std::shared_ptr<Transform::Transform> move_transform =
        std::make_shared<Transform::Transform>();

    move_transform->type = Transform::Type::FADE;
    move_transform->easing = Easing::linear;
    move_transform->time = {.start = start_time, .end = end_time};
    move_transform->before = sections[0];
    move_transform->after = sections[1];

    std::printf("%f %f \n", sections[0][0], sections[1][0]);
  }

  std::shared_ptr<Transform::Transform> invalid_transform =
      std::make_shared<Transform::Transform>();
  invalid_transform->type = Transform::Type::_INVALID;

  return invalid_transform;
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