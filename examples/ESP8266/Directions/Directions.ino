#include <GoogleMapsDirectionsApi.h>

/*******************************************************************
 *  An example of usisng the distance-matrix api to get            *
 *  travel time (with traffic) between two locations               *
 *                                                                 *
 *  Written by Brian Lough                                         *
 *******************************************************************/


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "PASSWORD";  // your network key
#define API_KEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your google apps API Token

WiFiClientSecure client;
GoogleMapsDirectionsApi api(API_KEY, client);

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
  DirectionsResponse response = api.directionsApi(origin, destination, departureTime, trafficModel);
  Serial.println("Response:");
  Serial.print("Summary: ");
  Serial.println(response.summary);

  Serial.print("Duration in Traffic text: ");
  Serial.println(response.durationTraffic_text);
  Serial.print("Duration in Traffic in minutes: ");
  Serial.println(response.durationTraffic_value);

  Serial.print("Normal duration text: ");
  Serial.println(response.duration_text);
  Serial.print("Normal duration in minutes: ");
  Serial.println(response.duration_value);

  Serial.print("Distance text: ");
  Serial.println(response.distance_text);
  Serial.print("Distance in meters: ");
  Serial.println(response.distance_value);
}

void loop() {

  if ((millis() > api_lasttime + api_mtbs))  {
    checkGoogleMaps();
    api_lasttime = millis();
  }
}
