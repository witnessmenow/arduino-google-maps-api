/*******************************************************************
 *  An example of using the distance-matrix API to get            *
 *  travel time (with traffic) between two locations (JSON V6)             *
 *                                                                 *
 *  Written by Brian Lough                                         *
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <GoogleMapsApi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! -------
#define API_KEY "xxxxxxxxxxxx"      // Your Google Apps API Token
char ssid[] = "xxxxxxxxxxxx";       // Your Network SSID (name)
char password[] = "xxxxxxxxxxxx";   // Your Network key

WiFiClientSecure client;
GoogleMapsApi api(API_KEY, client);

//Free Google Maps API only Allows Maximum 2500 "elements" a day, So careful you dont go over
int api_mtbs = 60000;    //mean time between api requests
long api_lasttime = 0;   //last time api request has been done
bool firstTime = true;

//Inputs
String origin = "Galway";
String destination = "Dublin,Ireland";
// For both Origin and Destination you should be
// able to pass multiple seperated by a |
// Example : Destination1|Destination2 etc

//Optional
String departureTime = "now";       //This can also be a timestamp, needs to be in the future for traffic info
String trafficModel = "best_guess"; //defaults to this anyways. see https://developers.google.com/maps/documentation/distance-matrix/intro#DistanceMatrixRequests for more info

void setup() 
{
	Serial.begin(115200);
  	WiFi.mode(WIFI_STA); // Set WiFi to Station Mode
  	WiFi.disconnect();   // Disconnect from an AP if it was Previously Connected
  	delay(100);

  	Serial.print("Connecting Wifi: ");
  	Serial.println(ssid);
  	WiFi.begin(ssid, password);
  	while (WiFi.status() != WL_CONNECTED) 
  	{
  		client.setInsecure();
    	Serial.print(".");
    	delay(500);
  	}

  	Serial.println("");
  	Serial.println("WiFi connected");
  	Serial.println("IP address: ");
  	IPAddress ip = WiFi.localIP();
  	Serial.println(ip);
}

void checkGoogleMaps() 
{
	Serial.println("Getting traffic for " + origin + " to " + destination);
 	String responseString = api.distanceMatrix(origin, destination, departureTime, trafficModel);
    DynamicJsonDocument jsonBuffer(1024);
    DeserializationError response = deserializeJson(jsonBuffer, responseString);
    JsonObject root = jsonBuffer.as<JsonObject>();

    if (!response) 
    {
       String status = root["status"];
        if(status =="OK") 
        {
          Serial.println("Status : " + status);
          String durationInTraffic = root["rows"][0]["elements"][0]["duration_in_traffic"]["text"];
          int durationInSeconds = root["rows"][0]["elements"][0]["duration"]["value"];
          int durationInTrafficInSeconds = root["rows"][0]["elements"][0]["duration_in_traffic"]["value"];
          int distanceInKm = root["rows"][0]["elements"][0]["distance"]["value"];
          Serial.println("Duration In Traffic: " + durationInTraffic + "(" + durationInTrafficInSeconds + ")" + "Distance:" + distanceInKm);

        }
        else 
        {
          Serial.println("Got an Error Status");
        }
    }
    else 
    {
      if(responseString == "")
      {
        Serial.println("No response, Probably Timed Out");
      } 
      else 
      {
        Serial.println("Failed to Parse JSON. Response : ");
        Serial.println(responseString);
      }
    }
}

void loop() 
{
	if ((millis() > api_lasttime + api_mtbs))  
  	{
    	checkGoogleMaps();
    	api_lasttime = millis();
  	}
}
