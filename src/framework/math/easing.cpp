#include <cmath>
#include <map>

namespace Easing {

using EasingFunction = float (*)(float, float, float, float);

float linear(float t, float b, float c, float d) { return c * t / d + b; }

float quad_in(float tt, float b, float c, float d) {
  float t = tt / d;
  return c * t * t + b;
}

float quad_out(float tt, float b, float c, float d) {
  float t = tt / d;
  return -c * t * (t - 2) + b;
}

float quad_in_out(float tt, float b, float c, float d) {
  float t = tt / d;
  t /= d / 2;

  if (t < 1) {
    return c / 2 * t * t + b;
  }

  t -= 1;
  return -c / 2 * (t * (t - 2) - 1) + b;
}

float quart_in(float tt, float b, float c, float d) {
  float t = tt / d;
  return c * t * t * t * t + b;
}

float quart_out(float tt, float b, float c, float d) {
  float t = tt / d;
  t -= 1;
  return -c * (t * t * t * t - 1) + b;
}

float quart_in_out(float tt, float b, float c, float d) {
  float t = tt / d;
  t /= d / 2;

  if (t < 1) {
    return c / 2 * t * t * t * t + b;
  }

  t -= 2;
  return -c / 2 * (t * t * t * t - 2) + b;
}

float quint_in(float tt, float b, float c, float d) {
  float t = tt / d;
  return c * t * t * t * t * t + b;
}

float quint_out(float tt, float b, float c, float d) {
  float t = tt / d;
  t -= 1;
  return c * (t * t * t * t * t + 1) + b;
}

float quint_in_out(float tt, float b, float c, float d) {
  float t = tt / d;
  t /= d / 2;

  if (t < 1) {
    return c / 2 * t * t * t * t * t + b;
  }

  t -= 2;
  return c / 2 * (t * t * t * t * t + 2) + b;
}

float sine_in(float t, float b, float c, float d) {
  return -c * std::cos(t / d * (M_PI / 2)) + c + b;
}

float sine_out(float t, float b, float c, float d) {
  return c * std::sin(t / d * (M_PI / 2)) + b;
}

float sine_in_out(float t, float b, float c, float d) {
  return -c / 2 * (std::cos(M_PI * t / d) - 1) + b;
}

float elastic_in(float _t, float b, float c, float d) {
  float s = 1.70158;
  float a = c;
  float t = _t;

  if (t == 0) {
    return b;
  }

  t /= d;

  if (t == 1) {
    return b + c;
  }

  float p = d * 0.3;
  if (a < std::abs(c)) {
    a = c;
    s = p / 4;
  } else {
    s = p / (2 * M_PI) * std::asin(c / a);
  }

  t -= 1;
  return -(a * std::pow(2, 10 * t) * std::sin((t * d - s) * (2 * M_PI) / p)) +
         b;
}

float elastic_out(float _t, float b, float c, float d) {
  float s, a = 1.70158;
  float t = _t;

  if (t == 0) {
    return b;
  }
  t /= d;
  if (t == 1) {
    return b + c;
  }

  float p = d * 0.3;
  if (a < std::abs(c)) {
    a, s = c, p / 4;
  } else {
    s = p / (2 * M_PI) * std::asin(c / a);
  }

  return a * std::pow(2, -10 * t) * std::sin((t * d - s) * (2 * M_PI) / p) + c +
         b;
}

float elastic_in_out(float _t, float b, float c, float d) {
  float s, a = 1.70158;
  float t = _t;

  if (t == 0) {
    return b;
  }

  t /= (d / 2);

  if (t == 2) {
    return b + c;
  }

  float p = d * (0.3 * 1.5);
  if (a < std::abs(c)) {
    a, s = c, p / 4;
  } else {
    s = p / (2 * M_PI) * std::asin(c / a);
  }

  if (t < 1) {
    t -= 1;
    return -0.5 * (a * std::pow(2, 10 * t) *
                   std::sin((t * d - s) * (2 * M_PI) / p)) +
           b;
  }

  return 0.0;
}

// Mapping
enum class EasingEnum {
  linear,
  ease_out,
  ease_in,
  quad_in,
  quad_out,
  quad_in_out,
  cubic_in,
  cubic_out,
  cubic_in_out,
  quart_in,
  quart_out,
  quart_in_out,
  quint_in,
  quint_out,
  quint_in_out,
  sine_in,
  sine_out,
  sine_in_out,
  expo_in,
  expo_out,
  expo_in_out,
  circ_in,
  circ_out,
  circ_in_out,
  elastic_in,
  elastic_out,
  elastic_half_out,
  elasic_quart_out,
  elastic_in_out,
  back_in,
  back_out,
  back_in_out,
  bounce_in,
  bounce_out,
  bounce_in_out
};

const std::map<int, EasingFunction> easing_maps = {
    {static_cast<int>(EasingEnum::linear), linear},
    {static_cast<int>(EasingEnum::quad_in), quad_in},
    {static_cast<int>(EasingEnum::quad_out), quad_out},
    {static_cast<int>(EasingEnum::quad_in_out), quad_in_out},
    {static_cast<int>(EasingEnum::quart_in), quart_in},
    {static_cast<int>(EasingEnum::quart_out), quart_out},
    {static_cast<int>(EasingEnum::quart_in_out), quart_in_out},
    {static_cast<int>(EasingEnum::quint_in), quint_in},
    {static_cast<int>(EasingEnum::quint_out), quint_out},
    {static_cast<int>(EasingEnum::quint_in_out), quint_in_out},
    {static_cast<int>(EasingEnum::sine_in), sine_in},
    {static_cast<int>(EasingEnum::sine_out), sine_out},
    {static_cast<int>(EasingEnum::sine_in_out), sine_in_out},
    {static_cast<int>(EasingEnum::elastic_in), elastic_in},
    {static_cast<int>(EasingEnum::elastic_out), elastic_out},
    {static_cast<int>(EasingEnum::elastic_in_out), elastic_in_out}};

EasingFunction from_int(int i) {
  auto result = easing_maps.find(i);

  if (result != easing_maps.end()) {
    return result->second;
  }

  return linear;
}

} // namespace Easing
