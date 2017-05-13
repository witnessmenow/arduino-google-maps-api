#!/bin/sh -eux

platformio ci $PWD/examples/$BOARDTYPE$EXAMPLE_FOLDER$EXAMPLE_NAME/$EXAMPLE_NAME.ino -l '.' -l '/home/travis/.platformio/lib/ArduinoJson_ID64' -l '/home/travis/.platformio/lib/JsonStreamingParser_ID561' -b $BOARD
