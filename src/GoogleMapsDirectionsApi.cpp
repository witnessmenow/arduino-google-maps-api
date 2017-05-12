
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
String parentsParent = "";
DirectionsResponse responseObject;

//**************************************************************************//
// This code is the JSON Parser code written by squix78 as part of his example, modified for this application //
// https://github.com/squix78/json-streaming-parser //

class DirectionsListener: public JsonListener {

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
  //Serial.println("whitespace");
}

void DirectionsListener::startDocument() {
  //Serial.println("start document");
}

void DirectionsListener::key(String key) {
	  currentKey = key;
}

void DirectionsListener::value(String value) {
  if (parentsParent == "legs") {
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
    } else {
      if(currentKey == "summary") {
    	  responseObject.summary = value;
      }
    }
  }
}

void DirectionsListener::endArray() {
  //Serial.println("end array. ");
}

void DirectionsListener::endObject() {
  currentParent = parentsParent;
  parentsParent = "";
  //Serial.println("end object. ");
}

void DirectionsListener::endDocument() {
  //Serial.println("end document. ");
}

void DirectionsListener::startArray() {
   //Serial.println("start array. ");
}

void DirectionsListener::startObject() {
  parentsParent = currentParent;
  currentParent = currentKey;
   //Serial.println("start object. ");
}
//*********** END of Json Parser code **************//

GoogleMapsDirectionsApi::GoogleMapsDirectionsApi(String apiKey, Client &client)	{
  _apiKey = apiKey;
  this->client = &client;
}

DirectionsResponse GoogleMapsDirectionsApi::sendGetToGoogleMapsDirections(String command) {

	Serial.println("starting sendGet function");
	JsonStreamingParser parser;
	DirectionsListener listener;
  responseObject = DirectionsResponse();
	parser.setListener(&listener);

	long now;
	bool avail;
	// Connect with google-maps api over ssl
	if (client->connect(GMAPI_HOST, GMAPI_SSL_PORT)) {
		Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("GET "+command);
		now=millis();
		avail=false;
		while (millis()-now<1500) {
			while (client->available()) {
				char c = client->read();
				// parsing code:
				// most of the work happens in the header code
				// at the top of this file
				parser.parse(c);

        //TODO: Check that we have values for everything we expect and break
      }
		}
	}

  return responseObject;
}

DirectionsResponse GoogleMapsDirectionsApi::directionsApi(String origin, String destination, String departureTime, String trafficModel) {
  String command="https://maps.googleapis.com/maps/api/directions/json?origin=" + origin + "&destination=" + destination; //If you can't find it(for example if you have a custom url) look here: https://www.google-maps.com/account_advanced
  if (departureTime != "") {
    command = command + "&departure_time=" + departureTime;
  }
  if (trafficModel != "") {
    command = command + "&traffic_model=" + trafficModel;
  }

  command = command + "&key=" + _apiKey;
  //Serial.println("command to send to google:" + command);
  return sendGetToGoogleMapsDirections(command);       //recieve reply from google-maps
}
