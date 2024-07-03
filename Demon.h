#pragma once

#pragma once
#include "includes.h"

struct Demon {
	Sprite D;
	int health, speed = 150, power = 3, damage = 50;
	float cooldown = 2, stamina = 10, AttackSpeed = 0.15;
	bool alive;
};

void Dcreate();
void Dset(int);
void Dmove(float, Sprite, int, int&);
void Ddraw(RenderWindow&);


