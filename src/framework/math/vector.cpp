#ifndef FUCKING_VECTOR
#define FUCKING_VECTOR

namespace Math {
template <typename T> class Vector2 {
public:
  T x;
  T y;

  Vector2<T> minus(const Vector2<T> &other) const {
    return {x - other.x, y - other.y};
  }
};

// osu! origin fuckery
enum OriginType {
  TOP_LEFT,
  TOP_CENTRE,
  TOP_RIGHT,

  CENTRE_LEFT,
  CENTRE,
  CENTRE_RIGHT,

  BOTTOM_LEFT,
  BOTTOM_CENTRE,
  BOTTOM_RIGHT,
};

const std::map<std::string, OriginType> ORIGIN_STRING_TO_ENUM_MAP = {
    {"TopLeft", TOP_LEFT},
    {"TopCenter", TOP_CENTRE},
    {"TopRight", TOP_RIGHT},
    {"CentreLeft", CENTRE_LEFT},
    {"Centre", CENTRE},
    {"CentreRight", CENTRE_RIGHT},
    {"BottomLeft", BOTTOM_LEFT},
    {"BottomCentre", BOTTOM_CENTRE},
    {"BottomRight", BOTTOM_RIGHT},
};

OriginType get_origin_from_str(std::string s) {
  auto origin = ORIGIN_STRING_TO_ENUM_MAP.find(s);

  if (origin != ORIGIN_STRING_TO_ENUM_MAP.end()) {
    return origin->second;
  }

  return OriginType::TOP_LEFT;
}

Vector2<float> get_offset(OriginType origin, Vector2<float> size) {
  switch (origin) {
  case TOP_LEFT:
    return {0.0f * size.x, 0.0f * size.y};
  case TOP_CENTRE:
    return {0.5f * size.x, -1.0f * size.y};
  case TOP_RIGHT:
    return {1.0f * size.x, -1.0f * size.y};
  case CENTRE_LEFT:
    return {0.0f * size.x, 0.5f * size.y};
  case CENTRE:
    return {0.5f * size.x, 0.5f * size.y};
  case CENTRE_RIGHT:
    return {1.0f * size.x, 0.5f * size.y};
  case BOTTOM_LEFT:
    return {0.0f * size.x, 1.0f * size.y};
  case BOTTOM_CENTRE:
    return {0.5f * size.x, 1.0f * size.y};
  case BOTTOM_RIGHT:
    return {1.0f * size.x, 1.0f * size.y};
  default:
    std::cerr << "Error: Invalid origin type" << std::endl;
    return {0.0f, 0.0f}; // Default to (0, 0) for invalid types
  }
}
} // namespace Math

#endif