#include "../../math/vector.cpp"
#include "sokol_gfx.h"
#include "sokol_gl.h"
#include "stb_image.h"
#include <string>

using SokolImage = sg_image;

class Size {
public:
  int width;
  int height;
  int channels;
};

class TextureInfo {
public:
  int id = -1;
  std::string path;
  std::string extension;
};

class Texture {
public:
  Size size;
  TextureInfo info;
  SokolImage image;

  void *data;
  bool valid;

  // FNs
  void initialize_sokol_image();
  void draw(Math::Vector2<float> position, Math::Vector2<float> size);

  // Debug
  Texture() { std::printf("[Texture] Created, %s \n", info.path.c_str()); }
  ~Texture() { std::printf("[Texture] Destroyed, %s \n", info.path.c_str()); }
};

void Texture::initialize_sokol_image() {
  std::printf("Init \n");
  sg_image_desc image_desc = {
      .width = size.width,
      .height = size.height,
      .num_mipmaps = 0,
      .label = info.path.c_str(),
  };

  image_desc.data.subimage[0][0] = sg_range{
      .ptr = data,
      .size = ulong(4 * size.width * size.height),
  };

  image = sg_make_image(&image_desc);
}

void Texture::draw(Math::Vector2<float> position, Math::Vector2<float> size) {
  sgl_enable_texture();
  sgl_texture(image, {});

  sgl_begin_quads();
  sgl_c4b(255, 255, 255, 255);

  sgl_v3f_t2f(position.x, position.y, 1.0f, 0.0f, 0.0f);
  sgl_v3f_t2f(position.x + size.x, position.y, 1.0f, 1.0f, 0.0f);
  sgl_v3f_t2f(position.x + size.x, position.y + size.x, 1.0f, 1.0f, 1.0f);
  sgl_v3f_t2f(position.x, position.y + size.x, 1.0f, 0.0f, 1.0f);

  sgl_end();
  sgl_disable_texture();
}

// Utils
namespace TextureUtils {

std::shared_ptr<Texture> load_texture(std::string path) {
  std::shared_ptr<Texture> texture = std::make_shared<Texture>();

  texture->info.path = path;
  texture->data = static_cast<void *>(
      stbi_load(path.c_str(), &texture->size.width, &texture->size.height,
                &texture->size.channels, 4));
  texture->valid = (texture->data);

  return texture;
}

} // namespace TextureUtils