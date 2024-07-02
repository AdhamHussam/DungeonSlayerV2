#pragma once
#pragma once
#include "includes.h"

struct BrainMole {
	Sprite BM;
	int health, speed = 100;
	bool alive;
};

void BMcreate();
void BMset(int);
void BMmove(float, Sprite, int, int&);
void BMdraw(RenderWindow&);