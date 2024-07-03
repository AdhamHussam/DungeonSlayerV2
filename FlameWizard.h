#pragma once
#include "includes.h"

struct FlameWizard {
	Sprite FW;
	int health, speed = 100;
	bool alive;
};

void FWcreate();
void FWset(int);
void FWmove(float, Sprite, int, int&);
void FWdraw(RenderWindow&);