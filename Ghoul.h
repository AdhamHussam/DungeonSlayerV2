#pragma once
#include "includes.h"

struct Ghoul {
	Sprite G;
	int health, speed = 100;
	bool alive;
};

void Gcreate();
void Gset(int);
void Gmove(float, Sprite, int, int&);
void Gdraw(RenderWindow&);