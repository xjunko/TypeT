#include "easing.cpp"
#include <vector>

namespace Transform {

enum Type {
  MOVE,
  MOVE_X,
  MOVE_Y,
  ANGLE,
  COLOR,
  FADE,
  SCALE,
  SCALE_MULTIPLY_BY,
  ADDITIVE,
  FLIP_VERT,
  FLIP_HORIZ,
  SCALE_VECTOR
};

class Transform {
public:
  Type type;
  Easing::EasingFunction easing = Easing::linear;

  float start;
  float end;
  float duration;

  std::vector<float> before;
  std::vector<float> after;

  // FNs
  float as_one(float);
};

float Transform::as_one(float time) {
  return easing(time - start, before[0], after[0] - before[0], duration);
}

} // namespace Transform