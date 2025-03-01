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

static uint8_t get_color_component(GColor color, char c) {
  switch(c) {
    case 'a':
    case 'A':
      return (color.argb & 0b11000000) >> 6;
    case 'r':
    case 'R':
      return (color.argb & 0b00110000) >> 4;
    case 'g':
    case 'G':
      return (color.argb & 0b00001100) >> 2;
    case 'b':
    case 'B':
      return (color.argb & 0b00000011);
    default:
      return 0;
  }
}

static uint8_t average_color_component(uint8_t a, uint8_t b) {
  int average = (((double)a + (double)b) / 2);
  return average & 0b00000011;
}

static uint8_t average_color_component_3(uint8_t a, uint8_t b, uint8_t c) {
  int average = (((double)a + (double)b + (double)c) / 3);
  return average & 0b00000011;
}

GColor8 mix_colors(GColor8 a, GColor8 b) {
  uint8_t averageA = average_color_component(get_color_component(a, 'a'), get_color_component(b, 'a'));
  uint8_t averageR = average_color_component(get_color_component(a, 'r'), get_color_component(b, 'r'));
  uint8_t averageG = average_color_component(get_color_component(a, 'g'), get_color_component(b, 'g'));
  uint8_t averageB = average_color_component(get_color_component(a, 'b'), get_color_component(b, 'b'));

  return (GColor8){.argb=(averageA << 6) | (averageR << 4) | (averageG << 2) | (averageB)};
}

GColor8 mix_colors_3(GColor8 a, GColor8 b, GColor8 c) {
  uint8_t averageA = average_color_component_3(get_color_component(a, 'a'), get_color_component(b, 'a'), get_color_component(c, 'a'));
  uint8_t averageR = average_color_component_3(get_color_component(a, 'r'), get_color_component(b, 'r'), get_color_component(c, 'r'));
  uint8_t averageG = average_color_component_3(get_color_component(a, 'g'), get_color_component(b, 'g'), get_color_component(c, 'g'));
  uint8_t averageB = average_color_component_3(get_color_component(a, 'b'), get_color_component(b, 'b'), get_color_component(c, 'b'));

  return (GColor8){.argb=(averageA << 6) | (averageR << 4) | (averageG << 2) | (averageB)};
}
