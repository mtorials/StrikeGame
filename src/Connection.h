#ifndef mds_connection
#define mds_connection

#include <FastLED.h>
#include <Util.h>

class Connection {
public:
    Connection() {};
    Connection(CRGB * led);
    void update(bool connected);
    void connect() { update(true); };
    bool isConnected() { return connected; };
    CRGB * getLED() { return led; }
private:
    bool connected = false;
    CRGB * led;
};

Connection::Connection(CRGB * led) {
    this->led = led;
};

void Connection::update(bool a) {

    this->connected = a;
    if (a) this->led->setColorCode(CRGB::Green);
    else this->led->setColorCode(CRGB::Black);
};

#endif