#pragma once
#include "includes.h"

struct FireKnight {
	Sprite FK;
	int health, speed = 100;
	bool alive;
};

void FKcreate();
void FKset(int);
void FKmove(float, Sprite, int, int&);
void FKdraw(RenderWindow&);
