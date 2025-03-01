/*
   This file is part of Pebble-BigVenn.

   Pebble-BigVenn is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Pebble-BigVenn is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Pebble-BigVenn .  If not, see <http://www.gnu.org/licenses/>.
 */

#include <pebble.h>

// Quick integer square root.
uint32_t isqrt(uint32_t n) {
  if (n == 0) {
    return 0;
  }
  uint32_t root = 0;
  uint32_t bit = 1 << 30;

  while (bit > n) {
    bit >>= 2;
  }

  while (bit != 0) {
    if (n >= root + bit) {
      n -= root + bit;
      root += 2 * bit;
    }
    root >>= 1;
    bit >>= 2;
  }
  return root;
}

// Distance between two points.
uint32_t two_point_distance(GPoint a, GPoint b) {
  return isqrt(((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)));
}

GPoint add_points(GPoint a, GPoint b) {
  return GPoint(a.x + b.x, a.y + b.y);
}

// Converts an angle and a distance to a cartesian point.
GPoint polar_to_point(int angle, int distance) {
  int x = distance
    * ((double)cos_lookup(DEG_TO_TRIGANGLE(angle)) / ((double)TRIG_MAX_ANGLE));
  int y = distance
    * ((double)sin_lookup(DEG_TO_TRIGANGLE(angle)) / ((double)TRIG_MAX_ANGLE));
  return GPoint(x, y);
}

// Converts a starting point, an angle, and a distance to a cartesian point.
GPoint polar_to_point_offset(GPoint offset, int angle, int distance) {
  return add_points(offset, polar_to_point(angle, distance));
}

// Checks if a point is inside a circle.
bool is_in_circle(GPoint p, GPoint circle_center, uint32_t radius) {
  return two_point_distance(p, circle_center) <= radius;
}
