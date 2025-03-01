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

#pragma once
#include <pebble.h>

uint32_t two_point_distance(GPoint a, GPoint b);
uint32_t isqrt(uint32_t n);
GPoint polar_to_point(int angle, int distance);
GPoint add_points(GPoint a, GPoint b);
GPoint polar_to_point_offset(GPoint offset, int angle, int distance);
bool is_in_circle(GPoint p, GPoint circle_center, uint32_t radius);
