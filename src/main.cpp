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
#include "framework/math/time.cpp"
#include "framework/storyboard/storyboard.cpp"

#include "framework/language/string.cpp"

namespace TypeT {

const sg_pass_action DEFAULT_PASS = {
    .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                  .clear_value = {.r = 0.1, .g = 0.1, .b = 0.1, .a = 1}}};

class Context {
public:
  std::unique_ptr<Time::Limiter> limiter;
  std::shared_ptr<Audio::BASS> audio_mixer;
  std::shared_ptr<Audio::Track> audio_song;

  std::unique_ptr<AssetManager> assets;
  std::unique_ptr<Storyboard> storyboard;
  std::filesystem::path storyboard_path;

  float time;

  // FNs
  void begin();
  void end();
  void update(float);
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

// Utils FN
std::filesystem::path find_audio_file_in_folder(std::filesystem::path path) {
  const auto supported_type = {"mp3", "ogg"};

  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    auto filename = StringUtils::trim_copy(entry.path().filename().string());

    for (const auto &file_format : supported_type) {
      if (StringUtils::ends_with(filename, file_format)) {
        return entry;
      }
    }
  }
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

  data->audio_song = Audio::new_track(
      find_audio_file_in_folder(data->storyboard_path.parent_path()),
      data->audio_mixer);

  // Init(s) fields
  data->limiter = std::make_unique<Time::Limiter>();
  data->limiter->fps = 60;

  data->assets = std::make_unique<AssetManager>();

  data->storyboard = std::make_unique<Storyboard>();
  data->storyboard->parse_file(data->storyboard_path);

  data->audio_song->play(); // Should be fine now.
}

void draw(void *user_data) {
  Context *ctx = static_cast<Context *>(user_data);
  ctx->time = ctx->audio_song->get_position();

  ctx->begin();

  ctx->storyboard->draw(ctx->time);

  ctx->end();
  ctx->limiter->sync();
}

void cleanup(void *user_data) {
  Context *data = static_cast<Context *>(user_data);

  sg_shutdown();
}

} // namespace TypeT

// Entrypoint
int main() {
  const std::filesystem::path storyboard_path =
      "/home/junko/Projects/TypeT/assets/Sakura No Zenya/Kushi - Sakura no "
      "Zenya (Speed Up Ver.) (Taeyang).osb";

  TypeT::Context *data = new TypeT::Context();
  data->storyboard_path = storyboard_path;

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