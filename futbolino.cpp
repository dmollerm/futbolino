#include "futbolino.h"

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
	// TODO: Change irA and irB pins and simplify the problem?
	if (checkDebounce(s.irA, _debounceIrA)){
		addGoal(_golsB);
	}
	if (checkDebounce(s.irB, _debounceIrB)){
		addGoal(_golsA);
	}
}

void Futbolino::updateScore(Buttons b){
	if (areAllButtonsPressed(b)){
		resetScore();
	} else {
		if (checkDebounce(b.plusA, _debounceButtonPlusA)){
			addGoal(_golsA);
		}
		if (checkDebounce(b.minusA, _debounceButtonMinusA)){
			addGoal(_golsA, -1);
		}
		if (checkDebounce(b.plusB, _debounceButtonPlusB)){
			addGoal(_golsB);
		}
		if (checkDebounce(b.minusB, _debounceButtonMinusB)){
			addGoal(_golsB, -1);
		}
	}
}

bool Futbolino::areAllButtonsPressed(Buttons b){
	return (b.plusA && b.minusA && b.plusB && b.minusB);
}

void Futbolino::resetScore(){
	_golsA = 0;
	_golsB = 0;
}

bool Futbolino::checkDebounce(bool &input, bool &debounce){
	if (!debounce) {
		if (input) {
			debounce = true;
			return true;
		}
	} else if (!input) {
		debounce = false;
	}
	return false;
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



