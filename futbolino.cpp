#include "futbolino.h"

Futbolino::Futbolino(Inputs in, MD_Parola *screen) {
	_in = in;
	_screen = screen;
}

void Futbolino::begin() {
	resetScore();
	_currentState = SERVE;
	_lastScored = UNDEFINED;
}

void Futbolino::loop() {

	Sensors s = readIRSensors();
	Buttons b = readButtons();

	switch (_currentState){
		case SERVE:
			chooseServerTeam(s, b);
			break;
		case PLAY:
			updateScore(s);
			updateScore(b);
			break;
		case WIN:
			break;
	}

	updateScreen();

	#ifdef _FUTBOLINO_H_DEBUG
	debug();
	#endif
}

void Futbolino::chooseServerTeam(Sensors s, Buttons b){

	if (checkDebounce(s.irA, _debounceIrA) ||
			checkDebounce(b.plusA, _debounceButtonPlusA)) {
		_lastScored = A;
		_currentState = PLAY;
	} else if (checkDebounce(s.irB, _debounceIrB) ||
			checkDebounce(b.plusB, _debounceButtonPlusB)) {
		_lastScored = B;
		_currentState = PLAY;
	}
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
		begin();
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
	// TODO: Proof of concept. Wins with 6, finish with 11
	if (team == 11){
		_currentState = WIN;
		resetScore();
	}
}

void Futbolino::updateScreen(){
	if (_screen->displayAnimate()){
		switch (_currentState){
			case SERVE:
				sprintf(_screenBufferA, TXT_SERVE);
				_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
				_screen->displayZoneText(1, _screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
				break;
			case PLAY:
				sprintf(_screenBufferA, "%d - %d", _golsA, _golsB);
				sprintf(_screenBufferB, "%d - %d", _golsB, _golsA);
				_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
				_screen->displayZoneText(1, _screenBufferB, CENTER, 0, 0, PRINT, NO_EFFECT);
				break;
			case WIN:
				sprintf(_screenBufferA, TXT_WIN);
				_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
				_screen->displayZoneText(1, _screenBufferA, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
				_currentState = SERVE;
				break;
		}
	}
	
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



