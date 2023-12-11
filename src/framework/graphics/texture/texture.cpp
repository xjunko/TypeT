#include <string>

#include "../../math/vector.cpp"
#include "../effects/additive.cpp"
#include "../effects/normal.cpp"
#include "sokol_gfx.h"
#include "sokol_gl.h"
#include "stb_image.h"

using SokolImage = sg_image;
using SokolSampler = sg_sampler;

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
  SokolSampler sampler;

  void *data;
  bool valid;

  // FNs
  void initialize_sokol_image();
  void draw(Math::Vector2<float> position, Math::Vector2<float> size,
            sg_color color, float angle, Math::OriginType origin,
            bool is_additive);

  // Debug
  ~Texture() {
    if (data) {
      stbi_image_free(data);

      if (image.id != 0 && sg_isvalid()) {
        sg_destroy_image(image);
      }

      std::printf("[Texture] Destroyed, %s \n", info.path.c_str());
    }
  }
};

void Texture::initialize_sokol_image() {
  // std::printf("Init \n");
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

  sg_sampler_desc smp_desc = {
      .min_filter = SG_FILTER_LINEAR,
      .mag_filter = SG_FILTER_LINEAR,
      .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
      .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
  };

  sampler = sg_make_sampler(&smp_desc);
}

// Multiple defs
void Texture::draw(Math::Vector2<float> position, Math::Vector2<float> size,
                   sg_color color, float angle, Math::OriginType origin,
                   bool is_additive) {

  // HACK:
  float scale_factor = 1.5;
  float offset_hack_x = 160.0f;
  float offset_hack_y = -2.0f;

  position.x *= scale_factor;
  position.y *= scale_factor;
  size.x *= scale_factor;
  size.y *= scale_factor;

  position.x += offset_hack_x;
  position.y += offset_hack_y;

  // HACK:
  float u0 = 0.0f / size.x;
  float v0 = 0.0f / size.y;
  float u1 = size.x / size.x;
  float v1 = size.y / size.y;

  float x0 = position.x;
  float y0 = position.y;

  float x1 = position.x + size.x;
  float y1 = position.y + size.y;

  if (is_additive) {
    sgl_load_pipeline(Additive::get());
  } else {
    sgl_load_pipeline(Normal::get());
  }

  sgl_enable_texture();
  sgl_texture(image, sampler);

  if (angle != 0) {
    float width = size.x;
    float height = size.y;

    sgl_push_matrix();

    // NOTE: CUCK CODE INCOMING
    switch (origin) {
    case Math::OriginType::TOP_LEFT:
      sgl_translate(x0, y0, 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0, -y0, 0);
      break;
    case Math::OriginType::TOP_CENTRE:
      sgl_translate(x0 + (width / 2), y0 - height, 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0 - (width / 2), -y0, 0);
      break;
    case Math::OriginType::TOP_RIGHT:
      sgl_translate(x0 + width, y0 - height, 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0 - width, -y0, 0);
      break;
    case Math::OriginType::CENTRE_LEFT:
      sgl_translate(x0, y0 + (height / 2), 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0, -y0 - (height / 2), 0);
      break;
    case Math::OriginType::CENTRE:
      sgl_translate(x0 + (width / 2), y0 + (height / 2), 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0 - (width / 2), -y0 - (height / 2), 0);
      break;
    case Math::OriginType::CENTRE_RIGHT:
      sgl_translate(x0 + width, y0 + (height / 2), 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0 - width, -y0 - (height / 2), 0);
      break;
    case Math::OriginType::BOTTOM_LEFT:
      sgl_translate(x0, y0 + height, 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0, -y0 - height, 0);
      break;
    case Math::OriginType::BOTTOM_CENTRE:
      sgl_translate(x0 + (width / 2), y0 + height, 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0 - (width / 2), -y0 - height, 0);
      break;
    case Math::OriginType::BOTTOM_RIGHT:
      sgl_translate(x0 + width, y0 + height, 0);
      sgl_rotate(-angle, 0, 0, 1);
      sgl_translate(-x0 - width, -y0 - height, 0);
      break;
    }
  }

  sgl_begin_quads();
  sgl_c4b(color.r, color.g, color.b, color.a);

  sgl_v3f_t2f(x0, y0, 1.0f, u0, v0);
  sgl_v3f_t2f(x1, y0, 1.0f, u1, v0);
  sgl_v3f_t2f(x1, y1, 1.0f, u1, v1);
  sgl_v3f_t2f(x0, y1, 1.0f, u0, v1);

  sgl_end();

  if (angle != 0) {
    sgl_pop_matrix();
  }

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