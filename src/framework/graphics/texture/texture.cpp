#include <string>

#include "../../math/vector.cpp"
#include "sokol_gfx.h"
#include "sokol_gl.h"
#include "stb_image.h"

// GLOBAL HACK -->
sg_pipeline_desc alpha_pipdesc = {
    .label = "alpha",
    .colors[0] = {.blend = sg_blend_state{
                      .enabled = true,
                      .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                      .dst_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                  }}};

sgl_pipeline alpha_pipeline;
bool alpha_init;
// --> GLOBAL HACK

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
            sg_color color);

  // Debug
  Texture() { std::printf("[Texture] Created, %s \n", info.path.c_str()); }
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

  sg_sampler_desc smp_desc = {
      .min_filter = SG_FILTER_LINEAR,
      .mag_filter = SG_FILTER_LINEAR,
      .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
      .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
  };

  sampler = sg_make_sampler(&smp_desc);
}

void Texture::draw(Math::Vector2<float> position, Math::Vector2<float> size,
                   sg_color color) {
  if (!alpha_init) {
    alpha_pipeline = sgl_make_pipeline(alpha_pipdesc);
    alpha_init = true;
  }

  // HACK:
  float scale_factor = 0.5;

  position.x *= scale_factor;
  position.y *= scale_factor;
  size.x *= scale_factor;
  size.y *= scale_factor;

  // HACK:
  float u0 = 0.0f / size.x;
  float v0 = 0.0f / size.y;
  float u1 = size.x / size.x;
  float v1 = size.y / size.y;

  float x0 = position.x;
  float y0 = position.y;

  float x1 = position.x + size.x;
  float y1 = position.y + size.y;

  sgl_load_pipeline(alpha_pipeline);

  sgl_enable_texture();
  sgl_texture(image, sampler);

  sgl_begin_quads();
  sgl_c4b(color.r, color.g, color.b, color.a);

  sgl_v3f_t2f(x0, y0, 1.0f, u0, v0);
  sgl_v3f_t2f(x1, y0, 1.0f, u1, v0);
  sgl_v3f_t2f(x1, y1, 1.0f, u1, v1);
  sgl_v3f_t2f(x0, y1, 1.0f, u0, v1);

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