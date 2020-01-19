#include <Arduino.h>
#include <FastLED.h>

#include "Connection.hpp"
#include "Field.hpp"

#define NUM_LEDS 98
#define NUM_LEDS_FIELDS 49
#define SIZE 8
#define SIZEMinusOne SIZE - 1

#define DEBUGMATRIX 
#define DEBUGCONNECTIONS
#define DEBUGFIELDS
#define DEBUGCONNECTIONMATRIX

CRGB leds[NUM_LEDS];
bool matrix[SIZE][SIZE];

int pinoutRows[SIZE] = {53, 51, 49, 47, 45, 43, 41, 39};
int pinoutCols[SIZE] = {38, 40, 42, 44, 46, 48, 50, 52};

Connection * connectionsHorizontily[7][7];
Connection * connectionsVerticaly[7][7];

Field * fields[SIZE][SIZE];

void checkButtonMatrix();
bool checkAround(int a, int b);
void checkFields();
void printConnectionMatrix();

void setup() {

  pinMode(13, OUTPUT);

  Serial.begin(9600);

  for (int i : pinoutRows) {

    pinMode(i, INPUT);
  }
  for (int i : pinoutCols) {

    pinMode(i, INPUT_PULLUP);
  }

  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS_FIELDS);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
  
  // Map LEDs to Connection (or without LEDs use default constructor)
  for (int i = 0; i < NUM_LEDS / 2; i++) {
    int a = (int)i / (SIZEMinusOne);
    int b = i % (SIZEMinusOne);
    connectionsHorizontily[a][b] = new Connection();
    connectionsVerticaly[a][b] = new Connection();
  }

  for (int i = 0; i < NUM_LEDS_FIELDS; i++) {
    int a = (int)(i / (SIZEMinusOne));
    int b = i % (SIZEMinusOne);
    if ((a % 2) == 1) {
      fields[a][abs(b - 6)] = new Field(&leds[i]);
    } else {
      fields[a][b] = new Field(&leds[i]);
    }
  }
}

int i = 0;
void loop() {
  checkButtonMatrix();
  digitalWrite(13, matrix[1][1]);

  bool change = false;
  for (int a = 0; a < SIZE; a++) {
    for (int b = 0; b < SIZE; b++) {

      if (checkAround(a, b)) change = true;
    }
  }

#ifdef DEBUGCONNECTIONMATRIX
  if (i > 100) {
    printConnectionMatrix();
    i = 0;
  }
#endif

  if (change) {

    checkFields();
  }

  FastLED.show();
  
  i++;
#ifdef DEBUGMATRIX
  delay(1000);
#endif
}

void checkFields() {
  for(int a = 0; a < SIZEMinusOne; a++) {
    for(int b = 0; b < SIZEMinusOne; b++) {
      
      int around = 0;
      if (connectionsHorizontily[a][b]->isConnected()) around++;
      if (b < SIZEMinusOne -1 && connectionsHorizontily[a][b + 1]->isConnected()) around++;
      if (connectionsVerticaly[a][b]->isConnected()) around++;
      if (a > 0 && connectionsVerticaly[a - 1][b]->isConnected()) around++;


#ifdef DEBUGFIELDS
        Serial.print(a);
        Serial.print("; ");
        Serial.print(b);
        Serial.print(": ");
        Serial.println(around);
#endif

      Field::State newState;

      switch(around) {
        
        case 0: newState = Field::OFF; break;
        case 1: newState = Field::S1; break;
        case 2: newState = Field::S2; break;
        case 3: newState = Field::S3; break;
        case 4: newState = Field::P1; break;
      }

      fields[a][b]->update(newState);
    }
  }
}

bool checkAround(int a, int b) {

  // If not pressed: return
  if (!matrix[a][b]) return false;
  
#ifdef DEBUGCONNECTIONS
  Serial.print(a);
  Serial.print(" ");
  Serial.println(b);
#endif

  // Check the buttons around it and connect Connection
  if (matrix[a + 1][b]) {

#ifdef DEBUGCONNECTIONS
    Serial.println("Connected!");
#endif

    connectionsHorizontily[a][b]->connect();
    return true;
  }
  if (matrix[a][b + 1]) {
    connectionsVerticaly[a][b]->connect();
    return true;
  }
  return false;
}

// IO Methods

void checkButtonMatrix() {
  for (int row = 0; row < SIZE; row++) {

  	pinMode(pinoutRows[row], OUTPUT);
    digitalWrite(pinoutRows[row], LOW);

#ifdef DEBUGMATRIX
    Serial.print(row);
    Serial.print(": ");
#endif

    for (int col = 0; col < SIZE; col++) {

#ifdef DEBUGMATRIX
      Serial.print(!digitalRead(pinoutCols[col]));
      Serial.print(", ");
#endif

      matrix[row][col] = !digitalRead(pinoutCols[col]);
    }
#ifdef DEBUGMATRIX
    Serial.println(";");
#endif
    pinMode(pinoutRows[row], INPUT);
  }
}

void printConnectionMatrix() {

  for (int row = 0; row < (SIZEMinusOne); row++) {

      Serial.print(row);
      Serial.print("(V): ");

      for (int col = 0; col < (SIZEMinusOne); col++) {

        Serial.print(connectionsVerticaly[row][col]->isConnected());
        Serial.print(", ");

        matrix[row][col] = !digitalRead(pinoutCols[col]);
      }
      Serial.println(";");
  }
  for (int row = 0; row < (SIZEMinusOne); row++) {
    
      Serial.print(row);
      Serial.print("(H): ");

      for (int col = 0; col < (SIZEMinusOne); col++) {

        Serial.print(connectionsHorizontily[row][col]->isConnected());
        Serial.print(", ");

        matrix[row][col] = !digitalRead(pinoutCols[col]);
      }
      Serial.println(";");
  }
}
