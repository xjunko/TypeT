#include <filesystem>
#include <list>
#include <string>

#include "texture.cpp"

const std::shared_ptr<Texture> INVALID_SPRITE = std::make_shared<Texture>();

class AssetManager {
public:
  std::list<std::shared_ptr<Texture>> textures;

  // FNs
  std::shared_ptr<Texture> load_image(std::string path);
};

std::shared_ptr<Texture> AssetManager::load_image(std::string path) {
  if (!std::filesystem::exists(path)) {
    std::printf("[Assets] Invalid path, returning. [%s]", path.c_str());
    return INVALID_SPRITE;
  }

  std::shared_ptr<Texture> texture = TextureUtils::load_texture(path);
  texture->initialize_sokol_image();

  return texture;
}