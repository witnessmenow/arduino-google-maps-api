
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

#include "GoogleMapsDirectionsApi.h"

String currentKey = "";
String currentParent = "";
int depthFromLegsKey = 0;
DirectionsResponse responseObject;
bool finishedLegsArray = false;
bool startedLegsArray = false;

//**************************************************************************//
// This code is the JSON Parser code written by squix78 as part of his example,
// modified for this application //
// https://github.com/squix78/json-streaming-parser //

class DirectionsListener : public JsonListener {
 public:
  virtual void whitespace(char c);

  virtual void startDocument();

  virtual void key(String key);

  virtual void value(String value);

  virtual void endArray();

  virtual void endObject();

  virtual void endDocument();

  virtual void startArray();

  virtual void startObject();
};

void DirectionsListener::whitespace(char c) {
  // Serial.println("whitespace");
}

void DirectionsListener::startDocument() {
  // Serial.println("start document");
}

void DirectionsListener::key(String key) { currentKey = key; }

void DirectionsListener::value(String value) {
  if (depthFromLegsKey == 1) {
    if (currentKey == "start_address") {
      responseObject.start_address = value;
    } else if (currentKey == "end_address") {
      responseObject.end_address = value;
    }
  } else if (depthFromLegsKey == 2) {
    if (currentParent == "distance") {
      if (currentKey == "text") {
        responseObject.distance_text = value;
      } else if (currentKey == "value") {
        responseObject.distance_value = value.toInt();
      }
    } else if (currentParent == "duration") {
      if (currentKey == "text") {
        responseObject.duration_text = value;
      } else if (currentKey == "value") {
        responseObject.duration_value = value.toInt();
      }
    } else if (currentParent == "duration_in_traffic") {
      if (currentKey == "text") {
        responseObject.durationTraffic_text = value;
      } else if (currentKey == "value") {
        responseObject.durationTraffic_value = value.toInt();
      }
    }
  }
}

void DirectionsListener::endArray() {
  finishedLegsArray = startedLegsArray && depthFromLegsKey == 0;
  // Serial.println("end array. ");
}

void DirectionsListener::endObject() {
  if (depthFromLegsKey > 0) {
    depthFromLegsKey--;
    // Serial.print("depth from leg. ");
    // Serial.println(depthFromLegsKey);
  }
  currentParent = "";
  // Serial.println("end object. ");
}

void DirectionsListener::endDocument() {
  // Serial.println("end document. ");
}

void DirectionsListener::startArray() {
  if (currentKey == "legs") {
    startedLegsArray = true;
    // Serial.println("start leg array. ");
  }
  // Serial.println("start array. ");
}

void DirectionsListener::startObject() {
  if (depthFromLegsKey > 0 || (startedLegsArray && !finishedLegsArray)) {
    depthFromLegsKey++;
    // Serial.print("depth from leg. ");
    // Serial.println(depthFromLegsKey);
  }
  if (currentKey == "legs") {
    depthFromLegsKey = 1;
  }

  currentParent = currentKey;
  // Serial.println("start object. ");
}
//*********** END of Json Parser code **************//

GoogleMapsDirectionsApi::GoogleMapsDirectionsApi(String apiKey,
                                                 Client &client) {
  _apiKey = apiKey;
  this->client = &client;
}

DirectionsResponse GoogleMapsDirectionsApi::sendGetToGoogleMapsDirections(
    String command) {
  Serial.println("starting sendGet function");
  JsonStreamingParser parser;
  DirectionsListener listener;
  responseObject = DirectionsResponse();
  depthFromLegsKey = 0;
  finishedLegsArray = false;
  startedLegsArray = false;
  parser.setListener(&listener);

  long now;
  bool avail;
  // Connect with google-maps api over ssl
  if (client->connect(GMAPI_HOST, GMAPI_SSL_PORT)) {
    // Serial.println(".... connected to server");
    char c;
    client->println("GET " + command);
    now = millis();
    while (millis() - now < 3000) {
      while (client->available()) {
        char c = client->read();
        // parsing code:
        // most of the work happens in the header code
        // at the top of this file
        parser.parse(c);

        if (finishedLegsArray) {
          // Serial.println("finished");
          return responseObject;
        }
      }
    }
  }

  return responseObject;
}

DirectionsResponse GoogleMapsDirectionsApi::directionsApi(
    String origin, String destination, DirectionsInputOptions options) {
  String command =
      "https://maps.googleapis.com/maps/api/directions/json?origin=" + origin +
      "&destination=" +
      destination;  // If you can't find it(for example if you have a custom
                    // url) look here:
                    // https://www.google-maps.com/account_advanced
  if (options.departureTime != "") {
    command = command + "&departure_time=" + options.departureTime;
  }
  if (options.trafficModel != "") {
    command = command + "&traffic_model=" + options.trafficModel;
  }
  if (options.waypoints != "") {
    command = command + "&waypoints=" + options.waypoints;
  }
  if (options.units != "") {
    command = command + "&units=" + options.units;
  }
  if (options.avoid != "") {
    command = command + "&avoid=" + options.avoid;
  }

  command = command + "&key=" + _apiKey;
  // Serial.println("command to send to google:" + command);
  return sendGetToGoogleMapsDirections(
      command);  // recieve reply from google-maps
}
