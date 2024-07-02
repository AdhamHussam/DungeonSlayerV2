#pragma once
#include "includes.h"

struct NightBrone {
	Sprite NB;
	int health, speed = 100, power = 3, damage = 10;
	float cooldown = 10, AttackSpeed = 0.15;
	bool alive;
};

void NBcreate();
void NBset(int);
void NBmove(float,Sprite,int,int&,bool&);
void NBdraw(RenderWindow&);