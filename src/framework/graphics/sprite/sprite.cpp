#include "../texture/texture_manager.cpp"
#include <array>
#include <string>
#include <vector>

#define MAX_TEXTURE_LIMIT 10

class Sprite {
public:
  Math::Vector2<float> position;
  Math::Vector2<float> size;

  std::vector<std::shared_ptr<Texture>> textures;
  int texture_frame;

  // FNs
  void draw();
};

void Sprite::draw() {
  //   if (!textures[texture_frame]) {
  //     return; // Welp, shit i guess.
  //   }

  textures[texture_frame]->draw(position, size);
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