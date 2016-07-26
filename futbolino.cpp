#include "futbolino.h"
#include <stdio.h>

Futbolino::Futbolino(Inputs in, MD_Parola *screenA) {
	_in = in;
	_screenA = screenA;
}

void Futbolino::begin() {
	_screenA->displayText(_screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
}

void Futbolino::loop() {

	Sensors s = readIRSensors();
	updateScore(s);
	Buttons b = readButtons();
	updateScore(b);

	// Screen update
	sprintf(_screenBufferA, "%d - %d", _golsA, _golsB);
	_screenA->displayReset();
	_screenA->displayAnimate();

	#ifdef _FUTBOLINO_H_DEBUG
	debug();
	#endif

	delay(200);
}

struct Sensors Futbolino::readIRSensors() {
	bool irA = analogRead(_in.PIN_IR_A) > IR_THRESHOLD;
	bool irB = analogRead(_in.PIN_IR_B) > IR_THRESHOLD;

	Sensors s = {irA, irB};
	return s;
}

struct Buttons Futbolino::readButtons() {
	bool plusA  = digitalRead(_in.PIN_TEAM_A_PLUS);
	bool minusA = digitalRead(_in.PIN_TEAM_A_MINUS);
	bool plusB  = digitalRead(_in.PIN_TEAM_B_PLUS);
	bool minusB = digitalRead(_in.PIN_TEAM_B_MINUS);

	Buttons b = {plusA, minusA, plusB, minusB};
	return b;
}

void Futbolino::updateScore(Sensors s){
	updateScore(s.irA, _debounceIrA, _golsB);
	updateScore(s.irB, _debounceIrB, _golsA);
}

void Futbolino::updateScore(Buttons b){
	if (b.plusA && b.minusA && b.plusB && b.minusB){
		_golsA = 0;
		_golsB = 0;
	} else {
		if (b.plusA) {
			addGoal(_golsA);
		}
		if (b.minusA) {
			addGoal(_golsA, -1);
		}
		if (b.plusB) {
			addGoal(_golsB);
		}
		if (b.minusB) {
			addGoal(_golsB, -1);
		}
	}
}

void Futbolino::updateScore(bool &ir, bool &debounce, int &team) {
	if (!debounce) {
		if (ir) {
			addGoal(team);
			debounce = true;
		}
	} else if (!ir) {
		debounce = false;
	}
}

void Futbolino::addGoal(int &team, int delta){
	team += delta;
}

#ifdef _FUTBOLINO_H_DEBUG
void Futbolino::debug() {
	Serial.print("SCORE A: ");
	Serial.print(_golsA);
	Serial.print(" | SCORE B: ");
	Serial.print(_golsB);
	Serial.println("");
}
#endif



