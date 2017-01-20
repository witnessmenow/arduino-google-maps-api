#include <GoogleMapsApi.h>

/*******************************************************************
 *  An example of bot that echos back any messages received        *
 *                                                                 *
 *  written by Giacarlo Bacchio (Gianbacchio on Github)            *
 *  adapted by Brian Lough                                         *
 *******************************************************************/


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "Mikrotik";       // your network SSID (name)
char password[] = "carolinebrian";  // your network key
#define API_KEY "AIzaSyCja04dWL2Jcwd9OIlAsJ0jntssAHUe_ik"  // your google apps API Token


WiFiClientSecure client;
GoogleMapsApi api(API_KEY, client);

//Free Google Maps Api only allows for 2500 "elements" a day, so carful you dont go over
int api_mtbs = 60000; //mean time between api requests
long api_lasttime = 0;   //last time api request has been done
bool firstTime = true;

//Inputs

String origin = "Galway";
String destination = "Dublin,Ireland";
// For both origin and destination you should be
// able to pass multiple seperated by a |
// e.g destination1|destination2 etc


//Optional
String departureTime = "now"; //This can also be a timestamp, needs to be in the future for traffic info
String trafficModel = "best_guess"; //defaults to this anyways. see https://developers.google.com/maps/documentation/distance-matrix/intro#DistanceMatrixRequests for more info

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  checkGoogleMaps();
}

void checkGoogleMaps() {
  Serial.println("Getting traffic for " + origin + " to " + destination);
    String responseString = api.distanceMatrix(origin, destination, departureTime, trafficModel);
    DynamicJsonBuffer jsonBuffer;
    JsonObject& response = jsonBuffer.parseObject(responseString);
    if (response.success()) {
      if (response.containsKey("rows")) {
        JsonObject& element = response["rows"][0]["elements"][0];
        String status = element["status"];
        if(status == "OK") {

          String distance = element["distance"]["text"];
          String duration = element["duration"]["text"];
          String durationInTraffic = element["duration_in_traffic"]["text"];

          Serial.println("Distance: " + distance);
          Serial.println("Duration: " + duration);
          Serial.println("Duration In Traffic: " + durationInTraffic);

        }
        else {
          Serial.println("Got an error status: " + status);
        }
      } else {
        Serial.println("Reponse did not contain rows");
      }
    } else {
      Serial.println("Failed to parse Json");
    }
}

void loop() {

  if ((millis() > api_lasttime + api_mtbs))  {
    checkGoogleMaps();
    api_lasttime = millis();
  }
}
