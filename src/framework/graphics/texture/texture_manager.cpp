#include <filesystem>
#include <map>
#include <string>

#include "texture.cpp"

const std::shared_ptr<Texture> INVALID_SPRITE = std::make_shared<Texture>();

class AssetManager {
public:
  std::map<std::string, std::shared_ptr<Texture>> textures;

  // FNs
  void update();
  std::shared_ptr<Texture> load_image(std::string path);
};

void AssetManager::update() {
  // Create a vector to store keys of textures to be removed
  std::vector<std::string> to_remove;

  for (auto const &element : textures) {
    if (element.second.use_count() <= 1) {
      to_remove.push_back(element.first);
    }
  }

  for (const auto &key : to_remove) {
    std::printf("[Assets] Texture removed: %s\n", key.c_str());
    textures.erase(key);
    }
}

std::shared_ptr<Texture> AssetManager::load_image(std::string path) {
  if (!std::filesystem::exists(path)) {
    std::printf("[Assets] Invalid path, returning. [%s]", path.c_str());
    return INVALID_SPRITE;
  }

  if (textures.find(path) == textures.end()) {
    textures[path] = TextureUtils::load_texture(path);
    textures[path]->initialize_sokol_image();
  }

  return textures[path];
}