#include "easing.cpp"
#include "time.cpp"
#include <memory>
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
  Time::Time<float> time;
  std::vector<float> before;
  std::vector<float> after;

  // FNs
  float as_one(float);
  std::tuple<float, float> as_two(float);
};

float Transform::as_one(float transform_time) {
  return easing(transform_time - time.start, before[0], after[0] - before[0],
                time.duration());
}

std::tuple<float, float> Transform::as_two(float transform_time) {
  return std::make_tuple(easing(transform_time - time.start, before[0],
                                after[0] - before[0], time.duration()),
                         easing(transform_time - time.start, before[1],
                                after[1] - before[1], time.duration()));
}

// Helper function
std::shared_ptr<Transform> create(Type type, Easing::EasingFunction easing,
                                  Time::Time<float> time,
                                  std::vector<float> before,
                                  std::vector<float> after) {
  std::shared_ptr<Transform> transform = std::make_shared<Transform>();
  transform->type = type;
  transform->time = time;
  transform->before = before;
  transform->after = after;

  return transform;
}

} // namespace Transform