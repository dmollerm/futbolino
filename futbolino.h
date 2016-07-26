#ifndef _FUTBOLINO_H
#define _FUTBOLINO_H

#include <MD_Parola.h>
#include <my_structs.h>

// #define _FUTBOLINO_H_DEBUG
#define SCREEN_BUF_SIZE 75

const int IR_THRESHOLD = 300;

class Futbolino {

public:
	Futbolino(Inputs in, MD_Parola *screenA);

	void begin();
	void loop();

private:

	Inputs _in;
	MD_Parola* _screenA;
	char _screenBufferA[SCREEN_BUF_SIZE];
	
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
	void updateScore(Buttons b);
	struct Sensors readIRSensors();
	void updateScore(Sensors s);
	void updateScore(bool &ir, bool &debounce, int &team);

	#ifdef _FUTBOLINO_H_DEBUG
	void debug();
	#endif

};

#endif