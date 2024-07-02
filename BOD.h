#pragma once
#include "includes.h"

struct BringerOfDeath
{
	Sprite BOD, spell;
	int health = 10;
	float speed = 100, cooldown = 5;
};

enum BOD {
	BODwalk, BODattack, BODcast, BODhurt, BODdie, BODspawn
};

// animation counters
int MovmentCounter[];
float MonsterCounter[];

extern BringerOfDeath BODmonsters[];
extern BringerOfDeath BODoriginal;
extern Texture BODtexture;
extern bool showBODSpell[], BODalive[];

IntRect getRect(int pos);
void UpdateMonsterAnimationCounter(int i, float st);
void Bringerwalk(int x, int y, int i);
void attack(int x, int y, int i);
void cast(int i);
void hurt(int i);
void die(int i);
void spawn(int i);
void MonstersMovment();
void SetMonsters();
void CreateMonsters();