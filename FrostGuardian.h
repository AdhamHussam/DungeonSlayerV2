#pragma once
#include "includes.h"

struct FrostGuardian {
	Sprite FG;
	int health, speed = 100, power = 3, damage = 35;
	float cooldown = 3, stamina = 10, AttackSpeed = 0.15;
	bool alive;
};

void FGcreate();
void FGset(int);
void FGmove(float, Sprite, int, int&);
void FGdraw(RenderWindow&);


