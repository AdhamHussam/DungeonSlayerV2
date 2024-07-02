#pragma once
#include "includes.h"

struct Cthulu {
	Sprite Ct;
	int health, speed = 100, power = 3, damage = 10;
	float cooldown = 5, stamina = 10, AttackSpeed = 0.15;
	bool alive;
};

void Ccreate();
void Cset(int);
void Ctmove(float, Sprite, int, int&);
void Cdraw(RenderWindow&);

