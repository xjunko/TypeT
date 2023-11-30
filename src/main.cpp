#include <iostream>

// Sokol
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"
#include "sokol_glue.h"
#include "sokol_log.h"

// Internal
#include "framework/audio/audio.cpp"
#include "framework/graphics/sprite/sprite.cpp"

namespace TypeT {

const sg_pass_action DEFAULT_PASS = {
    .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                  .clear_value = {.r = 255, .g = 255, .b = 255, .a = 255}}};

class Context {
public:
  std::unique_ptr<AssetManager> assets;
  std::unique_ptr<Sprite> sprite;

  std::shared_ptr<Audio::BASS> audio_mixer;
  std::shared_ptr<Audio::Track> audio_song;

  int time;
  std::shared_ptr<Transform::Transform> transform;

  // FNs
  void begin();
  void end();
};

// Context
void Context::begin() {
  sgl_defaults();
  sgl_matrix_mode_projection();
  sgl_ortho(0.0f, sapp_widthf(), sapp_heightf(), 0.0f, -1.0f, 1.0f);

  sg_begin_default_pass(DEFAULT_PASS, sapp_width(), sapp_height());
}

void Context::end() {
  sgl_draw();
  sg_end_pass();
  sg_commit();
}

// FNs
void initialize(void *user_data) {
  Context *data = static_cast<Context *>(user_data);

  // Graphic pipeline
  sg_desc desc = {
      .logger.func = slog_func,
      .context = sapp_sgcontext(),
  };
  sg_setup(&desc);

  sgl_desc_t sgl_desc = {};
  sgl_setup(&sgl_desc);

  // Audio pipeline
  data->audio_mixer = std::make_shared<Audio::BASS>();
  data->audio_mixer->initialize();

  data->audio_song = Audio::new_track("assets/Sakura No Zenya/[Nightcore].mp3",
                                      data->audio_mixer);
  data->audio_song->play();

  // Init(s) fields
  data->assets = std::make_unique<AssetManager>();

  data->sprite = SpriteUtils::create_sprite_from_image_path(
      "/home/junko/Downloads/junko.png");
  std::printf("Valid: %b \n", data->sprite->textures[0]->valid);

  // Transform TEST
  data->transform =
      Transform::create(Transform::Type::MOVE, Easing::linear, {100.0f, 200.0f},
                        {0.0f, 0.0f}, {320.0f, 240.0f});

  data->sprite->transforms.push_back(data->transform);
}

void draw(void *user_data) {
  Context *ctx = static_cast<Context *>(user_data);

  std::printf("%i \n", ctx->time);

  {
    ctx->time++;

    if (ctx->time >= 500) {
      ctx->sprite.reset();
    };
  }

  ctx->begin();

  if (ctx->time < 500) {
    ctx->sprite->update(float(ctx->time));
    ctx->sprite->draw();
  }

  ctx->end();
}

void cleanup(void *user_data) {
  Context *data = static_cast<Context *>(user_data);

  sg_shutdown();
}

} // namespace TypeT

// Entrypoint
int main() {
  TypeT::Context *data = new TypeT::Context();

  sapp_desc desc = {
      .user_data = data,
      .init_userdata_cb = &TypeT::initialize,
      .frame_userdata_cb = &TypeT::draw,
      .cleanup_userdata_cb = &TypeT::cleanup,
      .width = 1280,
      .height = 720,
      .window_title = "FUCK (sokol app)",
      .icon.sokol_default = true,
      .logger.func = slog_func,
  };

  sapp_run(&desc);

  delete data;

  return 0;
}