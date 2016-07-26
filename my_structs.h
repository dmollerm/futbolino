#ifndef _MY_STRUCTS_H
#define _MY_STRUCTS_H

typedef struct Buttons Buttons;
struct Buttons {
	bool plusA;
	bool minusA;
	bool plusB;
	bool minusB;
};

typedef struct Sensors Sensors;
struct Sensors {
	bool irA;
	bool irB;
};

typedef struct Inputs Inputs;
struct Inputs {
	int PIN_TEAM_A_PLUS;
	int PIN_TEAM_A_MINUS;
	int PIN_TEAM_B_PLUS;
	int PIN_TEAM_B_MINUS;
	int PIN_IR_A;
	int PIN_IR_B;
};


#endif