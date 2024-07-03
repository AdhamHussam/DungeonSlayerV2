#include "FrostGuardian.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture FGtexture;
FrostGuardian FGoriginal;
int FGmovementCounter[30];
float FGmonsterCounter[30], FGdeltatime;

enum FGenum {
    FG_walk, FG_attack, FG_hurt, FG_die, FG_spawn
};
FGenum FGstate[30];

IntRect FGgetRect(int pos) {
    int x = pos % 16;
    int y = pos / 16;
    return IntRect(x * 192, y * 128, 192, 128);
}


void FGupdateMonsterAnimationCounter(int i, float st = 0.15) {
    FGmonsterCounter[i] += FGdeltatime;
    if (FGmonsterCounter[i] >= st)
    {
        FGmonsterCounter[i] = 0;
        FGmovementCounter[i]++;
    }
}


void FGwalk(int i) {
    FGmovementCounter[i] %= 10;
    FGmonsters[i].FG.setTextureRect(FGgetRect(16 + FGmovementCounter[i]));
    Vector2f Direction = Player.getPosition() - FGmonsters[i].FG.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    FGmonsters[i].FG.move(Vector2f(Direction.x / (float)(abs(Direction.x) + abs(Direction.y)) * FGmonsters[i].speed * FGdeltatime,
        Direction.y / (float)(abs(Direction.x) + abs(Direction.y)) * FGmonsters[i].speed * FGdeltatime));
    FGupdateMonsterAnimationCounter(i);
    FGmovementCounter[i] %= 10;
}

void FGattack(int x, int y, int i) {
    FGmonsters[i].FG.setTextureRect(FGgetRect(32 + FGmovementCounter[i]));
    int initial = FGmovementCounter[i];
    FGupdateMonsterAnimationCounter(i, 0.1);
    if ((FGmovementCounter[i] == 7 && FGmovementCounter[i] > initial)) {
        if (abs(x) < 200 && abs(y) < 100) {
            Player_Health -= FGmonsters[i].damage;
            ishit = true;
        }
    }
    if (FGmovementCounter[i] == 14) {
        FGmovementCounter[i] = 0;
        FGmonsters[i].cooldown = 0.3;
        FGstate[i] = FGenum::FG_walk;
    }
}


void FGhurt(int i) {
    FGmonsters[i].FG.setTextureRect(FGgetRect(48 + FGmovementCounter[i]));
    FGupdateMonsterAnimationCounter(i, 0.1);
    if (FGmovementCounter[i] == 5) {
        FGmovementCounter[i] = 0;
        AblazeCharge += 4;
        FGstate[i] = FGenum::FG_walk;
    }
}


void FGdie(int i) {
    FGmonsters[i].FG.setTextureRect(FGgetRect(64 + FGmovementCounter[i]));
    FGupdateMonsterAnimationCounter(i, 0.2);
    if (FGmovementCounter[i] == 11)
        FGmonsters[i].alive = false, coinsCount += 30;
}


void FGcreate() {
    FGtexture.loadFromFile("enemies2/frost_guardian.png");
    FGoriginal.health = 500;
    FGoriginal.FG.setTexture(FGtexture);
    FGoriginal.FG.setTextureRect(FGgetRect(0));
    FGoriginal.FG.setOrigin(96, 64);
    FGoriginal.FG.setScale(3, 3);
    FGoriginal.FG.setPosition(300, 6700);
}

void FGset(int Rn) {
    for (int i = 0; i < FrostGuardianNumber; i++) {
        FGmonsters[i] = FGoriginal;
        FGmonsterCounter[i] = 0;
        FGmovementCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        FGmonsters[i].FG.setPosition(x, y);
        FGmonsters[i].alive = true;
        FGstate[i] = FGenum::FG_spawn;
    }
}


void FGmove(float time, Sprite p, int attct, int& PlayerHealth) {
    FGdeltatime = time;
    for (int i = 0; i < FrostGuardianNumber; i++) {

        FGmonsters[i].cooldown -= FGdeltatime;
        // check if alive
        if (!FGmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if C will die
        if (FGmonsters[i].health <= 0 && FGstate[i] != FGenum::FG_die) {
            FGmonsters[i].FG.setTexture(FGtexture);
            FGmovementCounter[i] = 0;
            FGstate[i] = FGenum::FG_die;
        }

        double x = p.getPosition().x - FGmonsters[i].FG.getPosition().x, y = FGmonsters[i].FG.getPosition().y - p.getPosition().y;

        // check if C is dying
        if (FGstate[i] == FGenum::FG_die) {
            FGdie(i);
            continue;
        }

        if (x < 0)
            FGmonsters[i].FG.setScale(Vector2f(3, 3));
        else
            FGmonsters[i].FG.setScale(Vector2f(-3, 3));

        // check if R is being attacked
        if (FGstate[i] != FGenum::FG_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            FGmonsters[i].health -= attct;
            FGmovementCounter[i] = 0;
            FGstate[i] = FGenum::FG_hurt;
        }

        // check if doing something
        if (FGstate[i] == FGenum::FG_hurt) {
            FGhurt(i);
            continue;
        }
        else if (FGstate[i] == FGenum::FG_attack && FGmonsters[i].cooldown <= 0) {
            FGattack(x, y, i);
            continue;
        }

        // make decision
        else if (abs(x) < 200 && abs(y) < 100) {
            FGstate[i] = FGenum::FG_attack;
            FGmovementCounter[i] = 0;
            FGattack(x, y, i);
        }

        else FGwalk(i);
    }
}

void FGdraw(RenderWindow& window) {
    for (int i = 0; i < FrostGuardianNumber; i++) {
        if (FGmonsters[i].alive) {
            window.draw(FGmonsters[i].FG);
            gui.drawBossHP(FGmonsters[i].health, 500);
        }
    }
}

