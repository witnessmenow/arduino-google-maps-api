# arduino-google-maps-api
A wrapper for the [Google Maps API](https://developers.google.com/maps/documentation) for Arduino (works on ESP8266)

Currently the only implemented method is a simple version of distance matrix but it can be easily extended. Please raise an issue if there is a method you are looking for.

## Getting a Google API key (Required!)

* Go to [this page](https://developers.google.com/maps/documentation/distance-matrix/)
* Scroll down and in "Quick Start Steps" section, click "Get Key"
* Make sure the following URL works for you in your browser (Change the key at the end!):
https://maps.googleapis.com/maps/api/distancematrix/json?origins=Galway,+Ireland&destinations=Dublin,Ireland&departure_time=now&traffic_model=best_guess&key=PutYourNewlyGeneratedKeyHere

## Installing

The downloaded code can be included as a new library into the IDE selecting the menu:

     Sketch / include Library / Add .Zip library

You also have to install the ArduinoJson library written by [Benoît Blanchon](https://github.com/bblanchon). Search for it on the Arduino Library manager or get it from [here](https://github.com/bblanchon/ArduinoJson).

Include GoogleMapsApi in your project:

    #include <GoogleMapsApi.h>

and pass it a API token and a SSL Client (See the examples for more details)

    #define API_KEY "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    // WiFiSSLClient client; //For 101 boards
    WiFiClientSecure client; //For ESP8266 boards
    GoogleMapsApi maps(API_KEY, client);

*NOTE:* This library has not been tested with the 101 boards as I do not have a compatible board. If you can help please let me know!
