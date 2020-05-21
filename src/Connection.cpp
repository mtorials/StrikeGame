#include "Connection.hpp"

Connection::Connection(){}

Connection::Connection(CRGB * led) {
    this->led = led;
}

void Connection::update(bool a) {

    this->connected = a;
    if (a) this->led->setColorCode(CRGB::Green);
    else this->led->setColorCode(CRGB::Black);
}

void Connection::connect() { 
   update(true); 
}

bool Connection::isConnected() { 
   return connected;
}

CRGB* Connection::getLED(){
   return led;
}