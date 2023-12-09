#pragma once

namespace Normal {

#include "sokol_gfx.h"
#include "sokol_gl.h"

// Global
bool INIT;
sgl_pipeline PIPELINE;

sgl_pipeline get() {
  if (!INIT) {
    sg_pipeline_desc desc = {
        .label = "alpha",
        .colors[0] = {.blend = sg_blend_state{
                          .enabled = true,
                          .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                          .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                      }}};
    PIPELINE = sgl_make_pipeline(desc);
    INIT = true;
  }

  return PIPELINE;
}

} // namespace Normal