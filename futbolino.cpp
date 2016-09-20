#include "futbolino.h"

char* goal_texts[] = {"Gas", "Bo", "100", "Dins", "Inside", "Mel", "Nyam", "Oju!"};

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

	_screenA->begin();
	_screenB->begin();

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
			updateFrom(s);
			updateFrom(b);
			break;
		case WIN:
			break;
		case END:
			break;
		default:
			;
	}

	updateScreen();
}

void Futbolino::chooseServerTeam(Sensors s, Buttons b){
	if (checkDebounce(s.irA, _debounceIrA) ||
			checkDebounce(b.plusA, _debounceButtonPlusA)) {
		DEBUG("team A scored");
		_lastScored = A;
		_screenA->setAnimation((char*)TXT_FIRSTBALL_A);
		_screenB->showScore(0, 0);
		_currentState = PLAY;
	} else if (checkDebounce(s.irB, _debounceIrB) ||
			checkDebounce(b.plusB, _debounceButtonPlusB)) {
		DEBUG("team B scored");
		_lastScored = B;
		_screenB->setAnimation((char*)TXT_FIRSTBALL_B);
		_screenA->showScore(0, 0);
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

void Futbolino::updateFrom(Sensors s){
	// TODO: Change irA and irB pins and simplify the problem?
	if (checkDebounce(s.irA, _debounceIrA)){
		addGoalB();
	}
	if (checkDebounce(s.irB, _debounceIrB)){
		addGoalA();
	}
}

void Futbolino::updateFrom(Buttons b){
	if (areAllButtonsPressed(b)){
		begin();
	} else {
		if (checkDebounce(b.plusA, _debounceButtonPlusA)){
			addGoalA();
		}
		if (checkDebounce(b.minusA, _debounceButtonMinusA)){
			subGoalA();
		}
		if (checkDebounce(b.plusB, _debounceButtonPlusB)){
			addGoalB();
		}
		if (checkDebounce(b.minusB, _debounceButtonMinusB)){
			subGoalB();
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

void Futbolino::addGoalA(){
	changeScore(_golsA);
	_lastScored = A;
	manageScoreIncrement();
}

void Futbolino::addGoalB(){
	changeScore(_golsB);
	_lastScored = B;
	manageScoreIncrement();
}

void Futbolino::subGoalA(){
	changeScore(_golsA, -1);
	showScoreInScreens();
}

void Futbolino::subGoalB(){
	changeScore(_golsB, -1);
	showScoreInScreens();
}

void Futbolino::manageScoreIncrement(){
	if (_golsB + _golsA == 11) {
		_currentState = END;

		char* end;
		if (_golsA > _golsB)
			end = (char*)TXT_END_A;
		else
			end = (char*)TXT_END_B;

		_screenA->setAnimation(end, callbackRestart);
		_screenB->setAnimation(end);
	} else {
		showScoreInScreens();

		char* txt;
		long rand = random(0,20);
		if (rand < 8){
			txt = goal_texts[rand];
			if (_lastScored == A){
				_screenA->setShortAnimation(txt);
			} else {
				_screenB->setShortAnimation(txt);
			}
		}
	}
}

void Futbolino::showScoreInScreens(){
	_screenA->showScore(_golsA, _golsB);
	_screenB->showScore(_golsB, _golsA);
}

void Futbolino::changeScore(int &team, int delta){
	if (team + delta >= 0 && _currentState == PLAY) {
		team += delta;
	}
}

void Futbolino::updateScreen(){
	bool animationFinished = _screen->displayAnimate();
	_screenA->update(animationFinished);
	_screenB->update(animationFinished);
}

extern Futbolino futbolino;

void callbackRestart() {
	futbolino.begin();
}
