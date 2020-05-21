#pragma once
#include <FastLED.h>

class Field {
public:
    enum State {
        OFF = CRGB::Black,
        S1 = CRGB::Gold,
        S2 = CRGB::Lime,
        S3 = CRGB::OrangeRed,
        P1 = CRGB::Blue,
        P2 = CRGB::Red
    };

    Field(CRGB * led);
    void update(State newState);
    State getState();
private:
    State state = OFF;
    CRGB * led;
};

