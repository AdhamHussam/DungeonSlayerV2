#pragma once
#include "includes.h"

struct CrystalMauler {
	Sprite CM;
	int health, speed = 100;
	bool alive;
};

void CMcreate();
void CMset(int);
void CMmove(float, Sprite, int, int&);
void CMdraw(RenderWindow&);
