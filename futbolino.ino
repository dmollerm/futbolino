// Screen includes
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


#include <stdio.h>
#include <my_structs.h>
#include <futbolino.h>

// Screen config and pins
#define SCREEN_BUF_SIZE 75
const int MAX_DEVICES = 4;
const int CLK_PIN = 12;
const int DATA_PIN = 11;
const int CS_PIN = 10;
MD_Parola SCREEN_A = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Input Pins
const Inputs in = {A0, A1, A2, A3, A5, A6};

// Apps
Futbolino futbolino = Futbolino(in, &SCREEN_A);


void setup() {
  Serial.begin(9600);

  // Input setup
  pinMode(in.PIN_TEAM_A_PLUS,  INPUT);
  pinMode(in.PIN_TEAM_A_MINUS, INPUT);
  pinMode(in.PIN_TEAM_B_PLUS,  INPUT);
  pinMode(in.PIN_TEAM_B_MINUS, INPUT);
  pinMode(in.PIN_IR_A,         INPUT);
  pinMode(in.PIN_IR_B,         INPUT);

  // Screen setup
  SCREEN_A.begin();
  futbolino.begin();
}


void loop() {
  futbolino.loop();
}