
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


#include "GoogleMapsApi.h"

GoogleMapsApi::GoogleMapsApi(String apiKey, Client &client)	{
  _apiKey = apiKey;
  this->client = &client;
}

String GoogleMapsApi::sendGetToGoogleMaps(String command) {
	String headers="";
  String body="";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
	long now;
	bool avail;
	// Connect with google-maps api over ssl
	if (client->connect(HOST, SSL_PORT)) {
		// Serial.println(".... connected to server");
		String a="";
		char c;
		int ch_count=0;
		client->println("GET "+command+"&key="+_apiKey);
		now=millis();
		avail=false;
		while (millis()-now<1500) {
			while (client->available()) {
				char c = client->read();
				//Serial.write(c);

        if(!finishedHeaders){
          if (currentLineIsBlank && c == '\n') {
            finishedHeaders = true;
          }
          else{
            headers = headers + c;

          }
        } else {
          if (ch_count < maxMessageLength)  {
            body=body+c;
            ch_count++;
  				}
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

				avail=true;
			}
			if (avail) {
				//Serial.println("Body:");
				//Serial.println(body);
				//Serial.println("END");
				break;
			}
		}
	}

  return body;
}

String GoogleMapsApi::distanceMatrix(String origin, String destination, String departureTime, String trafficModel) {
  String command="https://maps.googleapis.com/maps/api/distancematrix/json?origins=" + origin + "&destinations=" + destination; //If you can't find it(for example if you have a custom url) look here: https://www.google-maps.com/account_advanced
  if (departureTime != "") {
    command = command + "&departure_time=" + departureTime;
  }
  if (trafficModel != "") {
    command = command + "&traffic_model=" + trafficModel;
  }

  command = command + "&key=" + _apiKey;
  return sendGetToGoogleMaps(command);       //recieve reply from google-maps
}
