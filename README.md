# arduino-google-maps-api
A wrapper for the [Google Maps API](https://developers.google.com/maps/documentation) for Arduino (works on ESP8266). Available on the Arduino library manager.

There are now two different parts of the API implemented and they can be used separately (They are independent of each other)


# Directions API

[Link to Directions API documentation](https://developers.google.com/maps/documentation/directions/)

This is probably the better of the two APIs (which have similar functions)

### Main Features:
- Supports travel times (including traffic) between two locations
- Also supports waypoints (which basically travel time between A->B via C)
- Supports more optional inputs (avoid, units)
- Returns a struct object with the more useful info from the response
- Maybe slightly slower than DistanceMatrix as the response is very large (although the library minimises this, by only parsing the useful info)

### Example
[Example here](https://github.com/witnessmenow/arduino-google-maps-api/blob/master/examples/ESP8266/Direcitons/Directions.ino)

Note: that only one of the headers of this library is used: *GoogleMapsDirectionsApi.h*

### Required Libraries
The Directions part of the library requires [Squix78's Json Streamer Parser](https://github.com/squix78/json-streaming-parser), which can be installed via the arduino library manager.

### Getting a Google API key (Required!)

* Go to [this page](https://developers.google.com/maps/documentation/directions/)
* Scroll down and in "Quick Start Steps" section, click "Get Key"
* Make sure the following URL works for you in your browser (Change the key at the end!):
https://maps.googleapis.com/maps/api/directions/json?origin=Galway&destination=Dublin,Ireland&departure_time=now&waypoints=via:cork,ireland&key=PutYourNewlyGeneratedKeyHere


# Distance Matrix API

[Link to Distance Matrix API documentation](https://developers.google.com/maps/documentation/distance-matrix/)

This is the API that was implemented in the first version of the library.

### Main Features:
- Supports travel times (including traffic) between two locations
- Returns the full response from Google as a String in JSON format
- Maybe slightly faster than Directions as the response is much smaller

### Example
[Example here](https://github.com/witnessmenow/arduino-google-maps-api/blob/master/examples/ESP8266/DistanceMatrix/DistanceMatrix.ino)

Note: that only one of the headers of this library is used: *GoogleMapsApi.h*

### Required Libraries
The Distance Matrix part of the library requires [Benoît Blanchon's ArduinoJson library](https://github.com/bblanchon/ArduinoJson), which can be installed via the arduino library manager.

### Getting a Google API key (Required!)

* Go to [this page](https://developers.google.com/maps/documentation/distance-matrix/)
* Scroll down and in "Quick Start Steps" section, click "Get Key"
* Make sure the following URL works for you in your browser (Change the key at the end!):
https://maps.googleapis.com/maps/api/distancematrix/json?origins=Galway,+Ireland&destinations=Dublin,Ireland&departure_time=now&traffic_model=best_guess&key=PutYourNewlyGeneratedKeyHere
