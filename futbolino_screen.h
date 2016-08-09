#ifndef _FUTBOLINO_SCREEN_H
#define _FUTBOLINO_SCREEN_H

#include <MD_Parola.h>

#define _FUTBOLINO_SCREEN_H_DEBUG
#ifdef _FUTBOLINO_SCREEN_H_DEBUG
#define DEBUG(A) Serial.println(A)
#else
#define DEBUG(A)
#endif

#define SCORE_BUF_SIZE 7

class FutbolinoScreen {

public:
	FutbolinoScreen(MD_Parola *screen, int zone);

	void update(bool animationFinished);
	void setStaticText(char* text);
	void setAnimation(char* text);
	void setAnimation(char* text, void (*)());
	void showScore(int a, int b);

private:
	MD_Parola* _screen;
	char _score_buffer[SCORE_BUF_SIZE];
	int _zone;
	bool _animating;
	void (* _next_callback)();

	void _showScore();

};

#endif