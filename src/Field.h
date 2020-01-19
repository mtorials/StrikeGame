#ifndef mds_field
#define mds_field

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
    State getState() { return this->state; };
private:
    State state = OFF;
    CRGB * led;
};

Field::Field(CRGB * led) {

    this->led = led;
};

void Field::update(State newState) {

    this->led->setColorCode(newState);
    this->state = newState;
};

#endif