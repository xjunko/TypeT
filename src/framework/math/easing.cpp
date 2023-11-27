#include <cmath>

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

} // namespace Easing