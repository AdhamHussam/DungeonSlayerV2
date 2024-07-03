#pragma once
#include "includes.h"

struct ShardSoul {
	Sprite SS;
	int health, speed = 100;
	bool alive;
};

void SScreate();
void SSset(int);
void SSmove(float, Sprite, int, int&);
void SSdraw(RenderWindow&);