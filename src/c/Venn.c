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
#include "Venn.h"
#include "utils/MathUtils.h"
#include "utils/ColorUtils.h"

// Uncomment to output logs.
//#define LOG

// Uncomment to force a specific time.
//#define HOUR 10
//#define MINUTE 10
//#define SECOND 30

static Window *s_window;
static Layer *s_canvas_layer;

static GRect bounds;
static GPoint seconds_center;
static GPoint minutes_center;
static GPoint hours_center;
static GPoint seconds_dial_center;
static GPoint minutes_dial_center;
static GPoint hours_dial_center;

static uint32_t seconds_radius;
static uint32_t minutes_radius;
static uint32_t hours_radius;

static ClaySettings settings;

// Calculates the centers of each circle to be drawn.
static void set_centers(void) {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  int seconds = tick_time->tm_sec;
  int minutes = tick_time->tm_min;
  int hours = tick_time->tm_hour % 12;

#ifdef HOUR
  hours = HOUR;
#endif

#ifdef MINUTE
  minutes = MINUTE;
#endif

#ifdef SECOND
  seconds = SECOND;
#endif

  if (!settings.SecondsHand) {
    seconds = 0;
  }

  int angle;
  int distance;
  GPoint center = GPoint(bounds.size.w / 2, bounds.size.h / 2);

  if (settings.SecondsHand) {
    angle = ((double)seconds / 60 * 360) - 90;
    distance = seconds_radius - CENTER_MARGIN;
    seconds_center = polar_to_point_offset(center, angle, distance);

    distance = seconds_radius
      - CENTER_MARGIN
      + seconds_radius
      - DIAL_RADIUS * 2;
    seconds_dial_center = polar_to_point_offset(center, angle, distance);
  }

  angle = ((double)minutes / 60 * 360)
    + ((double)seconds / 60 * 360 / 60)
    - 90;
  distance = minutes_radius - CENTER_MARGIN;
  minutes_center = polar_to_point_offset(center, angle, distance);

  distance = minutes_radius - CENTER_MARGIN + minutes_radius - DIAL_RADIUS * 2;
  minutes_dial_center = polar_to_point_offset(center, angle, distance);

  angle = ((double)hours / 12 * 360)
    + ((double)minutes / 60 * 360 / 12)
    + ((double)seconds / 60 * 360 / 60 / 12)
    - 90;
  distance = hours_radius - CENTER_MARGIN;
  hours_center = polar_to_point_offset(center, angle, distance);

  distance = hours_radius - CENTER_MARGIN + hours_radius - DIAL_RADIUS * 2;
  hours_dial_center = polar_to_point_offset(center, angle, distance);
}

// Calculates the radiuses for the seconds, minutes, and hours hands.
// Size is dependant on the screen resolution.
static void set_radiuses(void) {
  seconds_radius = bounds.size.w / 4 + CENTER_MARGIN / 3;

  if (!settings.SecondsHand && settings.BiggerCircles) {
    minutes_radius = bounds.size.w / 4 + CENTER_MARGIN / 3;
    hours_radius = bounds.size.w / 5 + CENTER_MARGIN / 3;
  }
  else {
    minutes_radius = bounds.size.w / 5 + CENTER_MARGIN / 3;
    hours_radius = bounds.size.w / 6 + CENTER_MARGIN / 3;
  }
}

// Event called every tick (configurable to seconds or minutes).
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  set_centers();

#ifdef LOG
  int seconds = tick_time->tm_sec;
  int minutes = tick_time->tm_min;
  int hours = tick_time->tm_hour;
  APP_LOG(APP_LOG_LEVEL_INFO, "Time: %d:%d:%d", hours, minutes, seconds);
#endif

  layer_mark_dirty(s_canvas_layer);
}

// Draws the canvas.
static void canvas_update_proc(Layer *layer, GContext *ctx) {
  bool hasColor = false;

#ifdef PBL_COLOR
  hasColor = true;
#endif

  if (hasColor && !settings.ForceMonochrome) {
    for (int x = 0; x < bounds.size.w; x++) {
      for (int y = 0; y < bounds.size.h; y++) {
        GPoint p = GPoint(x, y);
        bool isInSeconds = false;
        bool isInMinutes = false;
        bool isInHours = false;
        bool isInCenter = false;
        bool isInSecondsDial = false;
        bool isInMinutesDial = false;
        bool isInHoursDial = false;

        if (settings.SecondsHand) {
          isInSeconds = is_in_circle(p, seconds_center, seconds_radius);
        }
        isInMinutes = is_in_circle(p, minutes_center, minutes_radius);
        isInHours = is_in_circle(p, hours_center, hours_radius);
        isInCenter = is_in_circle(p, GPoint(bounds.size.w / 2, bounds.size.h / 2), CENTER_RADIUS);

        if (settings.SecondsHand) {
          isInSecondsDial = isInSeconds
            && is_in_circle(p, seconds_dial_center, DIAL_RADIUS);
        }
        isInMinutesDial = isInMinutes
          && is_in_circle(p, minutes_dial_center, DIAL_RADIUS);
        isInHoursDial = isInHours
          && is_in_circle(p, hours_dial_center, DIAL_RADIUS);

        GColor color = settings.BackgroundColor;

        if (isInCenter) {
          if (two_point_distance(GPoint(bounds.size.w / 2, bounds.size.h / 2), p) < 4) {
            color = settings.BackgroundColor;
          } else {
            color = settings.BorderColor;
          }
        } else if (isInSeconds && isInMinutes && isInHours) {
          color = mix_colors_3(settings.SecondsColor, settings.MinutesColor, settings.HoursColor);

          if (isInSecondsDial && isInMinutesDial && isInHoursDial) {
            color = settings.BackgroundColor;
          }
          else if (isInSecondsDial && isInMinutesDial) {
            color = settings.HoursColor;
          }
          else if (isInSecondsDial && isInHoursDial) {
            color = settings.MinutesColor;
          }
          else if (isInMinutesDial && isInHoursDial) {
            color = settings.SecondsColor;
          }
          else if (isInSecondsDial) {
            color = mix_colors(settings.MinutesColor, settings.HoursColor);
          }
          else if (isInMinutesDial) {
            color = mix_colors(settings.SecondsColor, settings.HoursColor);
          }
          else if (isInHoursDial) {
            color = mix_colors(settings.SecondsColor, settings.MinutesColor);
          }
        }
        else if (isInSeconds && isInMinutes) {
          color = mix_colors(settings.SecondsColor, settings.MinutesColor);

          if (isInSecondsDial && isInMinutesDial) {
            color = settings.BackgroundColor;
          }
          else if (isInSecondsDial) {
            color = settings.MinutesColor;
          }
          else if (isInMinutesDial) {
            color = settings.SecondsColor;
          }
        }
        else if (isInSeconds && isInHours) {
          color = mix_colors(settings.SecondsColor, settings.HoursColor);
          if (isInSecondsDial && isInHoursDial) {
            color = settings.BackgroundColor;
          }
          else if (isInSecondsDial) {
            color = settings.HoursColor;
          }
          else if (isInHoursDial) {
            color = settings.SecondsColor;
          }
        }
        else if (isInMinutes && isInHours) {
          color = mix_colors(settings.MinutesColor, settings.HoursColor);
          if (isInMinutesDial && isInHoursDial) {
            color = settings.BackgroundColor;
          }
          else if (isInMinutesDial) {
            color = settings.HoursColor;
          }
          else if (isInHoursDial) {
            color = settings.MinutesColor;
          }
        }
        else if (isInSeconds) {
          color = settings.SecondsColor;
          if (isInSecondsDial) {
            color = settings.BackgroundColor;
          }
        }
        else if (isInMinutes) {
          color = settings.MinutesColor;
          if (isInMinutesDial) {
            color = settings.BackgroundColor;
          }
        }
        else if (isInHours) {
          color = settings.HoursColor;
          if (isInHoursDial) {
            color = settings.BackgroundColor;
          }
        }
        graphics_context_set_stroke_color(ctx, color);
        graphics_draw_pixel(ctx, GPoint(x, y));

      }
    }
  }
  else {
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_antialiased(ctx, false);

    graphics_context_set_fill_color(ctx, settings.BWBackgroundColor);
    graphics_fill_rect(ctx, bounds, 0, GCornersAll);

    graphics_context_set_stroke_color(ctx, settings.BWBorderColor);
    graphics_context_set_fill_color(ctx, settings.BWCirclesColor);

    if (settings.SecondsHand) {
      graphics_fill_circle(ctx, seconds_center, seconds_radius);
    }
    graphics_fill_circle(ctx, minutes_center, minutes_radius);
    graphics_fill_circle(ctx, hours_center, hours_radius);

    if (settings.SecondsHand) {
      graphics_draw_circle(ctx, seconds_center, seconds_radius);
    }
    graphics_draw_circle(ctx, minutes_center, minutes_radius);
    graphics_draw_circle(ctx, hours_center, hours_radius);

    graphics_context_set_fill_color(ctx, settings.BWBackgroundColor);
    if (settings.SecondsHand) {
      graphics_fill_circle(ctx, seconds_dial_center, DIAL_RADIUS);
    }
    graphics_fill_circle(ctx, minutes_dial_center, DIAL_RADIUS);
    graphics_fill_circle(ctx, hours_dial_center, DIAL_RADIUS);

    if (settings.SecondsHand) {
      graphics_draw_circle(ctx, seconds_dial_center, DIAL_RADIUS);
    }
    graphics_draw_circle(ctx, minutes_dial_center, DIAL_RADIUS);
    graphics_draw_circle(ctx, hours_dial_center, DIAL_RADIUS);

    graphics_fill_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), CENTER_RADIUS);
    graphics_draw_circle(ctx, GPoint(bounds.size.w / 2, bounds.size.h / 2), CENTER_RADIUS);
  }
}

// Main window load event.
static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  set_radiuses();
  set_centers();

  s_canvas_layer = layer_create(bounds);
  layer_add_child(window_layer, s_canvas_layer);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  if (settings.SecondsHand) {
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  }
  else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  }
}

// Main window unload event.
static void prv_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
}

// Saves settings to persistent storage.
static void prv_save_settings(void) {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Gets default settings.
static void prv_default_settings(void) {
  settings.SecondsHand = true;
  settings.ForceMonochrome = false;
  settings.BiggerCircles = false;
  settings.HoursColor = GColorChromeYellow;
  settings.MinutesColor = GColorVividCerulean;
  settings.SecondsColor = GColorFolly;
  settings.BackgroundColor = GColorWhite;
  settings.BorderColor = GColorBlack;
  settings.BWBackgroundColor = GColorWhite;
  settings.BWCirclesColor = GColorBlack;
  settings.BWBorderColor = GColorWhite;
}

// Loads settings from persistent storage.
static void prv_load_settings(void) {
  prv_default_settings();
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Event called on message received.
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
#ifdef LOG
  APP_LOG(APP_LOG_LEVEL_INFO, "Received message");
#endif

  Tuple *seconds_hand_t = dict_find(iter, MESSAGE_KEY_SecondsHand);
  Tuple *force_monochrome_t = dict_find(iter, MESSAGE_KEY_ForceMonochrome);
  Tuple *bigger_circles_t = dict_find(iter, MESSAGE_KEY_BiggerCircles);
  Tuple *hours_color_t = dict_find(iter, MESSAGE_KEY_HoursColor);
  Tuple *minutes_color_t = dict_find(iter, MESSAGE_KEY_MinutesColor);
  Tuple *seconds_color_t = dict_find(iter, MESSAGE_KEY_SecondsColor);
  Tuple *background_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  Tuple *border_color_t = dict_find(iter, MESSAGE_KEY_BorderColor);
  Tuple *bw_background_color_t = dict_find(iter, MESSAGE_KEY_BWBackgroundColor);
  Tuple *bw_circles_color_t = dict_find(iter, MESSAGE_KEY_BWCirclesColor);
  Tuple *bw_border_color_t = dict_find(iter, MESSAGE_KEY_BWBorderColor);

  if (seconds_hand_t) {
    settings.SecondsHand = seconds_hand_t->value->int32 == 1;
    tick_timer_service_unsubscribe();
    if (settings.SecondsHand) {
      tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    }
    else {
      tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    }
  }
  if (force_monochrome_t) {
    settings.ForceMonochrome = force_monochrome_t->value->int32 == 1;
  }
  if (bigger_circles_t) {
    settings.BiggerCircles = bigger_circles_t->value->int32 == 1;
  }
  if (hours_color_t) {
    settings.HoursColor = GColorFromHEX(hours_color_t->value->int32);
  }
  if (minutes_color_t) {
    settings.MinutesColor = GColorFromHEX(minutes_color_t->value->int32);
  }
  if (seconds_color_t) {
    settings.SecondsColor = GColorFromHEX(seconds_color_t->value->int32);
  }
  if (background_color_t) {
    settings.BackgroundColor = GColorFromHEX(background_color_t->value->int32);
  }
  if (border_color_t) {
    settings.BorderColor = GColorFromHEX(border_color_t->value->int32);
  }
  if (bw_background_color_t) {
    settings.BWBackgroundColor = GColorFromHEX(bw_background_color_t->value->int32);
  }
  if (bw_circles_color_t) {
    settings.BWCirclesColor = GColorFromHEX(bw_circles_color_t->value->int32);
  }
  if (bw_border_color_t) {
    settings.BWBorderColor = GColorFromHEX(bw_border_color_t->value->int32);
  }

  set_radiuses();
  set_centers();
  layer_mark_dirty(s_canvas_layer);

  prv_save_settings();
}

// Init watchface.
static void prv_init(void) {
  prv_load_settings();

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
      .load = prv_window_load,
      .unload = prv_window_unload,
      });
  const bool animated = true;
  window_stack_push(s_window, animated);

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
}

// Deinit watchface.
static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

#ifdef LOG
  APP_LOG(APP_LOG_LEVEL_INFO, "Done initializing, pushed window: %p", s_window);
#endif

  app_event_loop();

  prv_deinit();
}
