/**
 * @author B. Burak Payzun
 * @date 2022-01-01
 * 
 */
#ifndef ROVER_MATH_HELPERS_H
#define ROVER_MATH_HELPERS_H

#include "Arduino.h"

//#define RAD_TO_DEG 57.3248407643
//#define DEG_TO_RAD 0.01744444444

namespace rover{
  double clamp(double value, double lower, double upper)
  {
    return max(lower, min(value,upper));
  }

  double lerp(double a, double b, double t)
  {
    return a + (b - a) * t;
  }

  int map(double value, double current_from, double current_to, double target_from, double target_to)
  {
      return target_from + (value - current_from) / (current_to - current_from) * (target_to - target_from);
  }
}
/*
namespace rover
{
  inline double clamp(double value, double lower, double upper)
  {
    return max(lower, min(value, upper));
  }
  inline double lerp(double a, double b, double t)
  {
    return a + (b - a) * t;
  }
  inline double map(double value, double current_from, double current_to, double target_from, double target_to)
  {
    return target_from + (value - current_from) / (current_to - current_from) * (target_to - target_from);
  }

} // namespace rover
*/
#endif 