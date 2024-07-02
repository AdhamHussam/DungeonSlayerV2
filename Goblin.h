#pragma once
#include "includes.h"

enum GoblinState {
	GBLNwalk, GBLNhurt, GBLNdie, GBLNspawn, GBLNattack
};

struct Goblin {
	Sprite Gwalk, Gattack, Gdie, Ghit, Gcurrent, Gspawn;
	int health = 10;
	bool is_alive;
	float speed = 100, cooldown = 5;
	GoblinState goblin_state;
};

void create_goblins();
void set_goblins(int);
void draw_goblin(RenderWindow&, int);
IntRect get_rect(int, GoblinState);
void update_goblin_animation_counter(int, float);
void clear_goblin(int, GoblinState);
void goblin_spawn(int);
void goblin_walk(int, int, int);
void goblins_die(int);
void goblin_hurt(int);
void goblin_attack(int, int, int);
void goblins_move(int, int);

