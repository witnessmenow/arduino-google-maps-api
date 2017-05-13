#include <GoogleMapsDirectionsApi.h>

/*******************************************************************
 *  An example of usisng the directions api to get                 *
 *  travel time (with traffic) between two locations               *
 *  Example also shows setting a waypoint if you want a specific   *
 *  route                                                          *
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
unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;
bool firstTime = true;

//Inputs

String origin = "Galway";
String destination = "Dublin,Ireland";
String waypoints = "via:Cork,Ireland"; //You need to include the via: before your waypoint


//Optional
DirectionsInputOptions inputOptions;

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

  //These are all optional (although departureTime needed for traffic)
  inputOptions.departureTime = "now"; //can also be a future timestamp
  inputOptions.trafficModel = "best_guess"; //Defaults to this anyways
  inputOptions.avoid = "ferries";
  inputOptions.units = "metric";
}

void checkGoogleMaps() {
  Serial.println("Getting traffic for " + origin + " to " + destination);
  DirectionsResponse response = api.directionsApi(origin, destination, inputOptions);
  Serial.println("Response:");
  Serial.print("Trafic from ");
  Serial.print(response.start_address);
  Serial.print(" to ");
  Serial.println(response.end_address);

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
  unsigned long timeNow = millis();
  if ((timeNow > api_due_time))  {
    Serial.println("Check With No Waypoint");
    inputOptions.waypoints = "";
    checkGoogleMaps();
    Serial.print("Check With Waypoint: ");
    Serial.println(waypoints);
    inputOptions.waypoints = waypoints;
    checkGoogleMaps();
    api_due_time = timeNow + api_mtbs;
  }
}
