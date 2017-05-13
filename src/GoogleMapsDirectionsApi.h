/*

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

#ifndef GoogleMapsDirectionsApi_h
#define GoogleMapsDirectionsApi_h

#include <Arduino.h>
#include <Client.h>

#include "JsonStreamingParser.h"
#include "JsonListener.h"

#define GMAPI_HOST "maps.googleapis.com"
#define GMAPI_SSL_PORT 443

struct DirectionsResponse{
  int distance_value;
  String distance_text;
  int duration_value;
  String duration_text;
  int durationTraffic_value;
  String durationTraffic_text;
  String start_address;
  String end_address;
};

struct DirectionsInputOptions{
  String departureTime;
  String trafficModel;
  String waypoints;
  String units;
  String avoid;
};

class GoogleMapsDirectionsApi
{
  public:
    GoogleMapsDirectionsApi (String apiKey, Client &client);
    DirectionsResponse sendGetToGoogleMapsDirections(String command);
    DirectionsResponse directionsApi(String origin, String destination, DirectionsInputOptions options = DirectionsInputOptions());

  private:
    String _apiKey;
    Client *client;
};
#endif
