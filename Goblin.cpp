#include "Goblin.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture walkTexture, dieTexture, attackTexture, hitTexture, spawnTexture;
Goblin Base, goblins[30];
int animation_speed = 0.05, goblin_movement_counter[30];
float goblin_animation_counter[30];
Vector2f scale = { 1.7, 1.7 };
Vector2f oppos = { -1.7, 1.7 };

IntRect get_rect(int pos, GoblinState state) {
	int y = 0, x;
	switch (state) {
	case GoblinState::GBLNwalk:
	case GoblinState::GBLNattack:
		x = pos % 8;
		return IntRect(x * 150, y * 150, 150, 150);
	case GoblinState::GBLNhurt:
	case GoblinState::GBLNdie:
	case GoblinState::GBLNspawn:
		x = pos % 4;
		return IntRect(x * 150, y * 150, 150, 150);
	}
}

void update_goblin_animation_counter(int i, float s = 0.15) {
	goblin_animation_counter[i] += playerdeltatime;
	if (goblin_animation_counter[i] >= s)
	{
		goblin_animation_counter[i] = 0;
		goblin_movement_counter[i]++;
	}
}

void goblin_walk(int x, int y, int i) {
	goblins[i].Gcurrent.setTextureRect(get_rect(goblin_movement_counter[i], GoblinState::GBLNwalk));
	Vector2f Direction = Player.getPosition() - goblins[i].Gcurrent.getPosition();
	float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
	Vector2f norm_direction = Direction / magnitude;
	goblins[i].Gcurrent.move(Vector2f(norm_direction.x * goblins[i].speed * playerdeltatime, norm_direction.y * goblins[i].speed * playerdeltatime));
	CheckMonsterCollisions(goblins[i].Gcurrent, norm_direction.x * goblins[i].speed * playerdeltatime, norm_direction.y * goblins[i].speed * playerdeltatime);
	update_goblin_animation_counter(i);
	goblin_movement_counter[i] %= 8;
}

void clear_goblin(int i, GoblinState state) {
	if (state != GoblinState::GBLNattack) {
		goblins[i].Gattack.setPosition(-6000, -6000);
	}

	if (state != GoblinState::GBLNdie) {
		goblins[i].Gdie.setPosition(-6000, -6000);
	}

	if (state != GoblinState::GBLNwalk) {
		goblins[i].Gwalk.setPosition(-6000, -6000);
	}

	if (state != GoblinState::GBLNhurt) {
		goblins[i].Ghit.setPosition(-6000, -6000);
	}

	if (state != GoblinState::GBLNspawn) {
		goblins[i].Gspawn.setPosition(-6000, -6000);
	}
}

void goblin_spawn(int i) {
	goblins[i].Gcurrent.setTextureRect(get_rect(goblin_movement_counter[i], GoblinState::GBLNspawn));
	update_goblin_animation_counter(i);
	if (goblin_movement_counter[i] == 4) {
		goblin_movement_counter[i] = 0;
		goblins[i].goblin_state = GoblinState::GBLNwalk;
		Vector2f position = goblins[i].Gcurrent.getPosition();
		goblins[i].Gwalk.setPosition(position);
		clear_goblin(i, GoblinState::GBLNwalk);
		goblins[i].Gcurrent = goblins[i].Gwalk;
	}
}

void goblin_hurt(int i) {
	goblins[i].Gcurrent.setTextureRect(get_rect(((goblin_movement_counter[i] > 4) ? 8 - goblin_movement_counter[i] : goblin_movement_counter[i]), GoblinState::GBLNhurt));
	update_goblin_animation_counter(i);
	if (goblin_movement_counter[i] == 4) {
		goblin_movement_counter[i] = 0;
		goblins[i].goblin_state = GoblinState::GBLNwalk;
		Vector2f position = goblins[i].Gcurrent.getPosition();
		goblins[i].Gwalk.setPosition(position);
		clear_goblin(i, GoblinState::GBLNwalk);
		goblins[i].Gcurrent = goblins[i].Gwalk;
	}
}

void goblins_die(int i) {
	goblins[i].Gcurrent.setTextureRect(get_rect(goblin_movement_counter[i], GoblinState::GBLNdie));
	update_goblin_animation_counter(i);
	if (goblin_movement_counter[i] == 4)
		goblins[i].is_alive = false;
}

void goblin_attack(int x, int y, int i) {
	goblins[i].Gcurrent.setTextureRect(get_rect(goblin_movement_counter[i], GoblinState::GBLNattack));
	int initial = goblin_movement_counter[i];
	update_goblin_animation_counter(i);
	if (goblin_movement_counter[i] == 6 && goblin_movement_counter[i] > initial) {
		if (abs(x) < 200 && abs(y) < 30) {
			Player_Health -= 5;
			ishit = true;
		}
	}
	if (goblin_movement_counter[i] == 9) {
		goblin_movement_counter[i] = 0;
		goblins[i].goblin_state = GoblinState::GBLNwalk;
		Vector2f position = goblins[i].Gcurrent.getPosition();
		goblins[i].Gwalk.setPosition(position);
		clear_goblin(i, GoblinState::GBLNwalk);
		goblins[i].Gcurrent = goblins[i].Gwalk;
	}
}

void goblins_move(int num, int attct) {
	for (int i = 0; i < num; i++) {
		if (!goblins[i].is_alive)
			continue;

		room_cleared = false;
		if (goblins[i].goblin_state == GoblinState::GBLNdie) {
			goblins_die(i);
			continue;
		}

		if (goblins[i].goblin_state == GoblinState::GBLNspawn) {
			goblin_spawn(i);
			continue;
		}

		
		double x = Player.getPosition().x - goblins[i].Gcurrent.getPosition().x, y = goblins[i].Gcurrent.getPosition().y - Player.getPosition().y;
		if (x < 0)
			goblins[i].Gcurrent.setScale(Vector2f(-1.7, 1.7));
		else
			goblins[i].Gcurrent.setScale(Vector2f(1.7, 1.7));


		// passing time for cooldown
		goblins[i].cooldown -= playerdeltatime;

		if (goblins[i].goblin_state != GoblinState::GBLNhurt && abs(x) < 100 && abs(y) < 100 && attct) {
			goblins[i].health -= attct;
			goblins[i].goblin_state = GoblinState::GBLNhurt;
			Vector2f position = goblins[i].Gcurrent.getPosition();
			goblins[i].Ghit.setPosition(position);
			clear_goblin(i, GoblinState::GBLNhurt);
			goblins[i].Gcurrent = goblins[i].Ghit;
			goblin_movement_counter[i] = 0;
		}

		if (goblins[i].goblin_state == GoblinState::GBLNhurt) {
			if (goblins[i].health <= 0) {
				goblins[i].goblin_state = GoblinState::GBLNdie;
				Vector2f position = goblins[i].Gcurrent.getPosition();
				goblins[i].Gdie.setPosition(position);
				clear_goblin(i, GoblinState::GBLNdie);
				goblins[i].Gcurrent = goblins[i].Gdie;
				goblin_movement_counter[i] = 0;
				goblins_die(i);
			}
			else
				goblin_hurt(i);
			
			continue;
		}

		else if (goblins[i].goblin_state == GoblinState::GBLNattack) {
			goblin_attack(x, y, i);
			continue;
		}

		if (abs(x) < 300 && abs(y) < 30) {
			goblin_movement_counter[i] = 0;
			goblins[i].goblin_state = GoblinState::GBLNattack;
			Vector2f position = goblins[i].Gcurrent.getPosition();
			goblins[i].Gattack.setPosition(position);
			clear_goblin(i, GoblinState::GBLNattack);
			goblins[i].Gcurrent = goblins[i].Gattack;
			goblin_attack(x, y, i);
		}

		else if (goblins[i].goblin_state == GoblinState::GBLNwalk) {
			goblin_walk(x, y, i);
		}
	}
}

void set_goblins(int num) {
	for (int i = 0; i < num; i++) {
		goblins[i] = Base;
		goblins[i].is_alive = true;
		goblin_animation_counter[i] = 0;
		goblin_movement_counter[i] = 0;
		goblins[i].goblin_state = GoblinState::GBLNspawn;
		int x, y;
		RandPosition(x, y);
		goblins[i].Gcurrent.setPosition(x, y);
	}
}

void create_goblins() {
	walkTexture.loadFromFile("enemies/Goblin/Run.png");
	Base.health = 10;
	Base.Gwalk.setTexture(walkTexture);
	spawnTexture.loadFromFile("enemies/Goblin/Idle.png");
	Base.Gspawn.setTexture(spawnTexture);
	hitTexture.loadFromFile("enemies/Goblin/Take Hit.png");
	Base.Ghit.setTexture(hitTexture);
	attackTexture.loadFromFile("enemies/Goblin/Attack.png");
	Base.Gattack.setTexture(attackTexture);
	dieTexture.loadFromFile("enemies/Goblin/Death.png");
	Base.Gdie.setTexture(dieTexture);

	Base.Gcurrent = Base.Gspawn;
	Base.Gcurrent.setOrigin(105, 62);
	Base.Gcurrent.setScale(1.7, 1.7);
	Base.Gwalk.setOrigin(105, 62);
	Base.Gwalk.setScale(1.7, 1.7);
	Base.Gattack.setOrigin(105, 62);
	Base.Gattack.setScale(1.7, 1.7);
	Base.Gdie.setOrigin(105, 62);
	Base.Gdie.setScale(1.7, 1.7);
	Base.Ghit.setOrigin(105, 62);
	Base.Ghit.setScale(1.7, 1.7);
	Base.Gspawn.setOrigin(105, 62);
	Base.Gspawn.setScale(1.7, 1.7);
}

void draw_goblin(RenderWindow& window, int num) {
	for (int i = 0; i < num; i++) {
		if (goblins[i].is_alive) {
			window.draw(goblins[i].Gcurrent);
			gui.DrawMonsterHP(goblins[i].Gcurrent.getPosition(), goblins[i].health, 10, 30, 35);
		}
	}
}


