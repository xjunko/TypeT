#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>
#include <time.h>
#include <unistd.h>

#ifndef FRAMEWORK_MATH
#define FRAMEWORK_MATH

namespace Time {

using namespace std::chrono_literals;

// Utils
double sys_mono_now() {
  auto ts = new timespec();
  clock_gettime(CLOCK_MONOTONIC, ts);

  auto ret_val = double(ts->tv_sec) * 1000000000 + double(ts->tv_nsec);
  delete ts;

  return ret_val;
}

// Generic Time Struct
template <typename T> class Time {
public:
  T start;
  T end;

  // FNs
  T duration();
};

template <typename T> T Time<T>::duration() { return end - start; }

// Frame Rate Limiter
class Limiter {
private:
  double variable_yield_time;
  double last_time;

public:
  int fps;

  // FNs
  void sync();
};

void Limiter::sync() {
  if (fps <= 0) {
    return;
  }

  double sleep_time = 1000000000 / double(fps);
  double yield_time = std::min<double>(
      sleep_time,
      variable_yield_time + std::fmod(sleep_time, double(1000 * 1000)));

  double over_sleep = double(0);

  while (true) {
    double t = sys_mono_now() - last_time;

    if (t < sleep_time - yield_time) {
      std::this_thread::sleep_for(1ms);
    } else if (t < sleep_time) {
      std::this_thread::sleep_for(1ns); // 1 pico
    } else {
      over_sleep = t - sleep_time;
      break;
    }
  }

  last_time = sys_mono_now() - std::min<double>(over_sleep, sleep_time);

  if (over_sleep > variable_yield_time) {
    variable_yield_time =
        std::min<double>(variable_yield_time + 200, sleep_time);
  } else if (over_sleep < variable_yield_time - 200 * 1000) {
    variable_yield_time = std::min<double>(variable_yield_time - 2 * 1000, 0);
  }
}

} // namespace Time

#endif