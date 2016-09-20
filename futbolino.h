#ifndef _FUTBOLINO_H
#define _FUTBOLINO_H

#include <MD_Parola.h>
#include <my_structs.h>
#include <futbolino_screen.h>

#define _FUTBOLINO_H_DEBUG
#ifdef _FUTBOLINO_H_DEBUG
#define DEBUG(A) Serial.println(A)
#else
#define DEBUG(A)
#endif

#define SCREEN_BUF_SIZE 75

const int IR_THRESHOLD = 300;

enum GameState
{
	SERVE,
	PLAY,
	WIN,
	END
};

enum Team
{
	UNDEFINED,
	A,
	B
};

const char TXT_END[] = "Final de partit. Verds guanyen!";
const char TXT_WIN[] = "TEAM A WINS! YEAH!";
const char TXT_SERVE[] = "Saque";
const char TXT_FIRSTBALL[] = "Verds saquen";

void callbackRestart();

class Futbolino {

public:
	Futbolino(Inputs in, MD_Parola *screen);

	void begin();
	void loop();

private:

	Inputs _in;
	MD_Parola* _screen;
	char _screenBufferA[SCREEN_BUF_SIZE];
	char _screenBufferB[SCREEN_BUF_SIZE];

	FutbolinoScreen* _screenA;
	FutbolinoScreen* _screenB;

	GameState _currentState;
	Team _lastScored;
	
	bool _breakAnimation = false;
	int _golsA = 0;
	int _golsB = 0;
	bool _debounceIrA = false;
	bool _debounceIrB = false;
	bool _debounceButtonPlusA = false;
	bool _debounceButtonMinusA = false;
	bool _debounceButtonPlusB = false;
	bool _debounceButtonMinusB = false;

	void addGoal(int &team, int delta = 1);
	void readButton(int &ir, bool &debounce);
	struct Buttons readButtons();
	void chooseServerTeam(Sensors s, Buttons b);
	void updateScore(Buttons b);
	struct Sensors readIRSensors();
	void updateScore(Sensors s);
	bool checkDebounce(bool &input, bool &debounce);
	bool areAllButtonsPressed(Buttons b);
	void resetScore();

	void manageStates();
	void updateScreen();

};

#endif