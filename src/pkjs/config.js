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

module.exports = [
  {
    "type": "heading",
    "defaultValue": "Big Venn"
  },
  {
    "type": "text",
    "defaultValue": "<p>Feel free to contribute at the <a href=\"https://github.com/JavierRizzoA/Pebble-BigVenn\">github repo</a>.</p>"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Options"
      },
      {
        "type": "toggle",
        "label": "Seconds Hand",
        "description": "Hide the seconds hand to save battery.",
        "defaultValue": true,
        "messageKey": "SecondsHand"
      },
      {
        "type": "toggle",
        "label": "Force Monochrome",
        "description": "Forces the monochrome style on color-capable watches.",
        "defaultValue": false,
        "messageKey": "ForceMonochrome"
      },
      {
        "type": "toggle",
        "label": "Bigger Circles",
        "description": "If the seconds hand is hidden, this setting will make the hours and minutes hands bigger.",
        "defaultValue": false,
        "messageKey": "BiggerCircles"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
      },
      {
        "type": "color",
        "messageKey": "HoursColor",
        "label": "Hours Color",
        "description": "Color for the hours hand.",
        "defaultValue": "FFAA00"
      },
      {
        "type": "color",
        "messageKey": "MinutesColor",
        "label": "Minutes Color",
        "description": "Color for the minutes hand.",
        "defaultValue": "00AAFF"
      },
      {
        "type": "color",
        "messageKey": "SecondsColor",
        "label": "Seconds Color",
        "description": "Color for the seconds hand.",
        "defaultValue": "FF0055"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "label": "Background Color",
        "description": "Color for the background.",
        "defaultValue": "FFFFFF"
      },
      {
        "type": "color",
        "messageKey": "BorderColor",
        "label": "Border Color",
        "description": "Color for the border at the center of the hands.",
        "defaultValue": "000000"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];
