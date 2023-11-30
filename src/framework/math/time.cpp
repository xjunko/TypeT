#ifndef FRAMEWORK_MATH
#define FRAMEWORK_MATH

namespace Time {

template <typename T> class Time {
public:
  T start;
  T end;

  // FNs
  T duration();
};

template <typename T> T Time<T>::duration() { return end - start; }

} // namespace Time

#endif