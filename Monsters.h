#pragma once
#include "BOD.h"
#include "ArcanArcher.h"
#include "NightBorne.h"
#include "Goblin.h"
#include "cthulu.h"
#include "FlameWizard.h"
#include "Cacodaemon.h"
#include "FireKnight.h"

struct Coordinates
{
	int x;
	int y;
};

struct Cell
{
	int cost;
	Coordinates node;
};

inline bool operator>(const Cell& lhs, const Cell& rhs) {
	// Compare based on cost
	return lhs.cost > rhs.cost;
}

struct ParentsCostPair
{
	vector<vector<Coordinates>> parents;
	vector<vector<int>> cost;
};

void SetMonstersSprites();
void SetMonstersWave();
void MoveMonsters();
void ShowMonsters();
void ResetMonsters();