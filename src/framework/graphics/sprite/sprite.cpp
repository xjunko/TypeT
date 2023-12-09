#pragma once
#include <array>
#include <set>
#include <string>
#include <vector>

#include "../../math/time.cpp"
#include "../../math/transform.cpp"
#include "../texture/texture_manager.cpp"

#define CIRCLE_PI 3.141592653589793238462643383279502884L
#define MAX_TEXTURE_LIMIT 10

class Sprite {
public:
  Math::OriginType origin;
  Math::Vector2<float> position;
  Math::Vector2<float> size;
  Time::Time<float> time;
  float angle;

  std::vector<std::shared_ptr<Transform::Transform>> transforms;
  std::vector<std::shared_ptr<Texture>> textures;
  int texture_frame;

  bool is_additive;
  sg_color color;

  // FNs
  void draw();
  void update(float);
  void apply_transform(std::shared_ptr<Transform::Transform>, float);
  void reset_to_transforms();

  Sprite()
      : origin(Math::OriginType::TOP_LEFT), position(), size(), time(),
        angle(0.0f), textures(), texture_frame(0), is_additive(false),
        color({255, 255, 255, 255}) {
    textures.reserve(MAX_TEXTURE_LIMIT);
  }

  ~Sprite() {
    transforms.clear();
    textures.clear();
  }
};

// Internal
void Sprite::apply_transform(
    std::shared_ptr<Transform::Transform> current_transform,
    float current_time) {
  switch (current_transform->type) {

  case Transform::Type::MOVE: {
    auto [x, y] = current_transform->as_two(current_time);
    position.x = x;
    position.y = y;
    break;
  }

  case Transform::Type::MOVE_X: {
    float value = current_transform->as_one(current_time);
    position.x = value;
    break;
  }

  case Transform::Type::MOVE_Y: {
    float value = current_transform->as_one(current_time);
    position.y = value;
    break;
  }

  case Transform::Type::FADE: {
    color.a = current_transform->as_one(current_time);
    break;
  }

  case Transform::Type::COLOR: {
    auto [r, g, b] = current_transform->as_three(current_time);
    color.r = r;
    color.g = g;
    color.b = b;
    break;
  }

  case Transform::Type::SCALE_VECTOR: {
    auto [x, y] = current_transform->as_two(current_time);
    size.x = textures[texture_frame]->size.width * x;
    size.y = textures[texture_frame]->size.height * y;
    break;
  }

  case Transform::Type::SCALE_MULTIPLY_BY: {
    float multiplier = current_transform->as_one(current_time);
    size.x = textures[texture_frame]->size.width * multiplier;
    size.y = textures[texture_frame]->size.height * multiplier;
    break;
  }

  case Transform::Type::ADDITIVE: {
    is_additive = true;
    exit(1);
    break;
  }

  case Transform::Type::ANGLE: {
    angle = (current_transform->as_one(current_time));
  }
  }
}

void Sprite::reset_to_transforms() {
  std::set<Transform::Type> handled;

  for (auto const &transform : transforms) {
    if (handled.find(transform->type) == handled.end()) {
      apply_transform(transform, transform->time.start);
      handled.insert(transform->type);
    }
  }
}

// Update
const float MAX_TIME_CATCH_UP = 100.0f;
void Sprite::update(float time) {
  for (const auto &transform : transforms) {
    if (time < transform->time.start) {
      break;
    }

    if (time <= transform->time.end + MAX_TIME_CATCH_UP) {
      apply_transform(transform, std::min(time, transform->time.end));
    }
  }
}

// Draw
void Sprite::draw() {
  if (!textures[texture_frame]) {
    std::printf("INVALID TEXTUER!!!");
    return; // Welp, shit i guess.
  }

  auto offset_origin = Math::get_offset(origin, size);
  auto offset_position = position.minus(offset_origin);

  textures[texture_frame]->draw(offset_position, size, color, angle, origin,
                                is_additive);
}

// Helper FNs
namespace SpriteUtils {

std::unique_ptr<Sprite>
create_sprite_from_image_path(std::string path_to_file) {
  std::shared_ptr<Texture> texture = TextureUtils::load_texture(path_to_file);
  texture->initialize_sokol_image();

  if (!texture) {
    // TODO: Not sure how to handle this exactly yet.
    std::printf("[Sprite] Failed to load image from %s", path_to_file.c_str());
  }

  std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();

  sprite.get()->size.x = float(texture->size.width);
  sprite->size.y = float(texture->size.height);

  sprite->textures.push_back(texture);
  sprite.get()->textures.push_back(texture);

  std::printf("Position: %f | %f \n", sprite->position.x, sprite->position.y);
  std::printf("Size: %f | %f\n", sprite->size.x, sprite->size.y);

  return sprite;
}

} // namespace SpriteUtils