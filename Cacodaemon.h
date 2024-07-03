#pragma once
#include "includes.h"

struct Cacodaemon {
	Sprite CD;
	int health, speed = 100;
	bool alive;
};

void CDcreate();
void CDset(int);
void CDmove(float, Sprite, int, int&);
void CDdraw(RenderWindow&);