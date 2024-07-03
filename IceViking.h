#pragma once
#include "includes.h"

struct IceViking {
	Sprite IV;
	int health, speed = 100;
	bool alive;
};

void IVcreate();
void IVset(int);
void IVmove(float, Sprite, int, int&);
void IVdraw(RenderWindow&);