#include "futbolino.h"

Futbolino::Futbolino(Inputs in, MD_Parola *screen) {
	_in = in;
	_screen = screen;

	_screenA = new FutbolinoScreen(_screen, 0);
	_screenB = new FutbolinoScreen(_screen, 1);
}

void Futbolino::begin() {
	resetScore();
	_currentState = SERVE;
	_lastScored = UNDEFINED;

	_screenA->setStaticText((char*)TXT_SERVE);
	_screenB->setStaticText((char*)TXT_SERVE);
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
		default:
			;
	}

	updateScreen();
	// manageStates();
}

void Futbolino::chooseServerTeam(Sensors s, Buttons b){
	if (checkDebounce(s.irA, _debounceIrA) ||
			checkDebounce(b.plusA, _debounceButtonPlusA)) {
		DEBUG("team A scored");
		_lastScored = A;
		_screenA->setAnimation((char*)TXT_CONGRATS);
		_screenB->setAnimation((char*)TXT_CALM_SERVE);
		_currentState = PLAY;
	} else if (checkDebounce(s.irB, _debounceIrB) ||
			checkDebounce(b.plusB, _debounceButtonPlusB)) {
		DEBUG("team B scored");
		_lastScored = B;
		_screenA->setAnimation((char*)TXT_CALM_SERVE);
		_screenB->setAnimation((char*)TXT_CONGRATS);
		_currentState = PLAY;
	}
}

struct Sensors Futbolino::readIRSensors() {
	// IR SENSORS ARE TEMPORALLY DISABLED, AND ARE ALWAYS OFF
	// bool irA = analogRead(_in.PIN_IR_A) > IR_THRESHOLD;
	// bool irB = analogRead(_in.PIN_IR_B) > IR_THRESHOLD;
	bool irA = false;
	bool irB = false;

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
	if (team + delta >= 0 && _currentState == PLAY) {
		team += delta;
		_breakAnimation = true;
	}

	if (_golsB + _golsA == 11) {
		_currentState = WIN;
		_screenA->setAnimation((char*)TXT_WIN, callbackRestart);
		_screenB->setAnimation((char*)TXT_WIN);
	} else {
		_screenA->showScore(_golsA, _golsB);
		_screenB->showScore(_golsB, _golsA);
	}

}

// void Futbolino::manageStates() {
// 	switch (_currentState){
// 		case SERVE:
// 			if (_lastScored != UNDEFINED) {
// 				DEBUG(__FUNCTION__);
// 				_currentState = PLAY;
// 			}
// 			break;
// 		case PLAY:
// 			// TODO: Proof of concept. Wins with 6, finish with 11
// 			if (_golsB + _golsA == 11) {
// 				_currentState = WIN;
// 			}
// 			break;
// 		case WIN:
// 			begin();
// 			break;
// 		default:
// 			;
// 	}
// }

void Futbolino::updateScreen(){
	bool animationFinished = _screen->displayAnimate();
	_screenA->update(animationFinished);
	_screenB->update(animationFinished);


	// if (_screen->displayAnimate() || _breakAnimation) {
	// 	DEBUG(__FUNCTION__);
	// 	_breakAnimation = false;
	// 	switch (_currentState){
	// 		case SERVE:
	// 			switch (_lastScored) {
	// 				case UNDEFINED:
	// 					sprintf(_screenBufferA, TXT_SERVE);
	// 					_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
	// 					_screen->displayZoneText(1, _screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
	// 					break;
	// 				case A:
	// 					sprintf(_screenBufferA, TXT_CONGRATS);
	// 					sprintf(_screenBufferB, TXT_CALM_SERVE);
	// 					_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	// 					_screen->displayZoneText(1, _screenBufferB, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	// 					break;
	// 				case B:
	// 					sprintf(_screenBufferA, TXT_CALM_SERVE);
	// 					sprintf(_screenBufferB, TXT_CONGRATS);
	// 					_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	// 					_screen->displayZoneText(1, _screenBufferB, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	// 					break;
	// 			}
	// 			break;
	// 		case PLAY:
	// 			sprintf(_screenBufferA, "%d - %d", _golsA, _golsB);
	// 			sprintf(_screenBufferB, "%d - %d", _golsB, _golsA);
	// 			_screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
	// 			_screen->displayZoneText(1, _screenBufferB, CENTER, 0, 0, PRINT, NO_EFFECT);
	// 			break;
	// 		case WIN:
	// 			// sprintf(_screenBufferA, TXT_WIN);
	// 			// _screen->displayZoneText(0, _screenBufferA, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	// 			// _screen->displayZoneText(1, _screenBufferA, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	// 			_screenA->setAnimation((char*)TXT_WIN);
	// 			_screenB->setAnimation((char*)TXT_WIN);
	// 			break;
	// 		default:
	// 			;
	// 	}
	// 	manageStates();
	// }
}

extern Futbolino futbolino;

void callbackRestart() {
	futbolino.begin();
}
