/*
Copyright (c) 2015 Giancarlo Bacchio. All right reserved.

TelegramBot - Library to create your own Telegram Bot using
ESP8266 on Arduino IDE.
Ref. Library at https:github/esp8266/Arduino

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef GoogleMapsApi_h
#define GoogleMapsApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define HOST "maps.googleapis.com"
#define SSL_PORT 443

class GoogleMapsApi {
 public:
  GoogleMapsApi(String apiKey, Client &client);
  String sendGetToGoogleMaps(String command);
  String distanceMatrix(String origin, String destination,
                        String departureTime = "", String trafficModel = "");

 private:
  // JsonObject * parseUpdates(String response);
  String _apiKey;
  Client *client;
  const int maxMessageLength = 1500;
  bool checkForOkResponse(String response);
};

#endif
