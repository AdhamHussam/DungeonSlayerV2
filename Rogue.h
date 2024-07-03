#pragma once
#include "includes.h"

struct Rogue {
	Sprite R;
	int health, speed = 100, power = 3, damage = 25;
	float cooldown = 12,stamina = 10, AttackSpeed = 0.15;
	bool alive;
};

void Rcreate();
void Rset(int);
void Rmove(float, Sprite, int, int&);
void Rdraw(RenderWindow&);