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

#define SETTINGS_KEY 1
#define CENTER_MARGIN 15
#define CENTER_RADIUS 5
#define DIAL_RADIUS 3

typedef struct ClaySettings {
  bool SecondsHand;
  bool ForceMonochrome;
  bool BiggerCircles;
  GColor HoursColor;
  GColor MinutesColor;
  GColor SecondsColor;
  GColor BackgroundColor;
  GColor BorderColor;
} __attribute__((__packed__)) ClaySettings;
