#include "Field.hpp"

Field::Field(CRGB * led) {

    this->led = led;
}

void Field::update(State newState) {

    this->led->setColorCode(newState);
    this->state = newState;
}

Field::State Field::getState() { 
   return this->state;
}