#include "futbolino_screen.h"

FutbolinoScreen::FutbolinoScreen(MD_Parola *screen, int zone) {
	_screen = screen;
	_zone = zone;
	_animating = false;
	sprintf(_score_buffer, "0 - 0");
}

void FutbolinoScreen::update(bool animationFinished){
	if (animationFinished && _animating && _screen->getZoneStatus(_zone)){
		if (_next_callback){
			_next_callback();
			_next_callback = 0;
		} else {
			_showScore();
		}	
	}
}

void FutbolinoScreen::setStaticText(char* text){
	_animating = false;
	_screen->displayZoneText(_zone, text, CENTER, 0, 0, PRINT, NO_EFFECT);
}

void FutbolinoScreen::setAnimation(char* text){
	_animating = true;
	_screen->displayZoneText(_zone, text, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
}

void FutbolinoScreen::setAnimation(char* text, void (*function)()){
	_animating = true;
	_screen->displayZoneText(_zone, text, CENTER, 0, 0, SCROLL_LEFT, SCROLL_LEFT);
	_next_callback = function;
}

void FutbolinoScreen::_showScore(){
	setStaticText(_score_buffer);
}

void FutbolinoScreen::showScore(int a, int b){
	sprintf(_score_buffer, "%d - %d", a, b);
	_showScore();
}
