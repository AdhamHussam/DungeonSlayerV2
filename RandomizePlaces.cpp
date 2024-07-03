#include "RandomizePlaces.h"

void RandPosition(int& x, int& y) {
	//srand(time(nullptr));
	if (current_room == 1){
		x = rand() % 1300 - 700;
		y = rand() % 1100 + 4800;
	}
	else if (current_room == 2) {
		x = rand() % 1100 - 650;
		y = rand() % 1100 + 3000;
	}
	else if (current_room == 3) {
		x = rand() % 2000 - 1200;
		y = rand() % 500 + 1800;
	}
	else if (current_room == 4) {
		x = rand() % 1000 - 650;
		y = rand() % 1000 - 50;
	}
	else if (current_room == 5) {
		x = rand() % 1500 - 800;
		y = rand() % 700 - 1900;
	}
}

void MonsterNumber() {
	ifstream file("NumberOfMonsters.csv");
	string row;
	for (int i = 0; i <= current_room + (level - 1) * 6; i++)
		getline(file, row);
	stringstream ss(row);
	string cell;
	getline(ss, cell, ',');
	SkeletonNumber = stoi(cell);
	getline(ss, cell, ',');
	BrainMoleNumber = stoi(cell);
	getline(ss, cell, ',');
	ShardSoulNumber = stoi(cell);
	getline(ss, cell, ',');
	BODnumber = stoi(cell);
	getline(ss, cell, ',');
	NightBroneNumber = stoi(cell);
	getline(ss, cell, ',');
	ArcaneArcherNumber = stoi(cell);
	getline(ss, cell, ',');
	RogueNumber = stoi(cell);
	getline(ss, cell, ',');
	GoblinNumber = stoi(cell);
	getline(ss, cell, ',');
	CthuluNumber = stoi(cell);
	getline(ss, cell, ',');
	FrostGuardianNumber = stoi(cell);
	getline(ss, cell, ',');
	DemonNumber = stoi(cell);
	getline(ss, cell, ',');
	FlameWizardNumber = stoi(cell);
	getline(ss, cell, ',');
	CacodaemonNumber = stoi(cell);
}

void CheckMonsterCollisions(Sprite& Monster, float x, float y) {
	// right
	if(x > 0){
		for (int i = 0; i < 25; i++)
			if (Monster.getGlobalBounds().intersects(right_borders[i].getGlobalBounds())) {
				Monster.move(Vector2f(-x, 0));
				return;
			}
	}
	else{
		//left
		for (int i = 0; i < 25; i++)
			if (Monster.getGlobalBounds().intersects(left_borders[i].getGlobalBounds())) {
				Monster.move(Vector2f(-x, 0));
				return;
			}
	}

	if (y < 0) {
		// up
		for (int i = 0; i < 24; i++)
			if (Monster.getGlobalBounds().intersects(up_borders[i].getGlobalBounds())) {
				Monster.move(Vector2f(0, -y));
				return;
			}
	}
	else {
		//down
		for (int i = 0; i < 26; i++)
			if (Monster.getGlobalBounds().intersects(down_borders[i].getGlobalBounds())) {
				Monster.move(Vector2f(0, -y));
				return;
			}
	}
}