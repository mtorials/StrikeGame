#pragma once
#include <FastLED.h>

class Connection {
public:
    Connection();
    Connection(CRGB * led);
    void update(bool connected);
    void connect();
    bool isConnected();
    CRGB* getLED();
private:
    bool connected = false;
    CRGB* led;
};