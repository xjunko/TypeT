#pragma once

namespace Additive {

#include "sokol_gfx.h"
#include "sokol_gl.h"

// Global
bool INIT;
sgl_pipeline PIPELINE;

sgl_pipeline get() {
  if (!INIT) {
    sg_pipeline_desc desc = {
        .label = "additive",
        .colors[0] = {.blend = sg_blend_state{
                          .enabled = true,
                          .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                          .dst_factor_rgb = SG_BLENDFACTOR_ONE,
                      }}};
    PIPELINE = sgl_make_pipeline(desc);
    INIT = true;
  }

  return PIPELINE;
}

} // namespace Additive
