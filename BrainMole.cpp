#include "BrainMole.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture BMtexture;
BrainMole BMoriginal;
int BMmovmentCounter[30];
float BMmonBMterCounter[30], BMdeltatime;

enum BMenum {
    BM_walk, BM_attack, BM_hurt, BM_die
};
BMenum BMtate[30];

IntRect BMgetRect(int poBM) {
    int x = poBM % 7;
    int y = poBM / 7;
    return IntRect(x * 32, y * 32, 32, 32);
}

void BMupdateMonBMterAnimationCounter(int i, float BMt = 0.15) {
    BMmonBMterCounter[i] += BMdeltatime;
    if (BMmonBMterCounter[i] >= BMt)
    {
        BMmonBMterCounter[i] = 0;
        BMmovmentCounter[i]++;
    }
}

void BMwalk(int i) {
    BMmovmentCounter[i] %= 8;
    BMmonsters[i].BM.setTextureRect(BMgetRect(14 + BMmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - BMmonsters[i].BM.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    BMmonsters[i].BM.move(Vector2f(norm_direction.x * BMmonsters[i].speed * BMdeltatime, norm_direction.y * BMmonsters[i].speed * BMdeltatime));
    CheckMonsterCollisions(BMmonsters[i].BM, norm_direction.x * BMmonsters[i].speed * BMdeltatime, norm_direction.y * BMmonsters[i].speed * BMdeltatime);
    BMupdateMonBMterAnimationCounter(i);
    BMmovmentCounter[i] %= 4;
}

void BMattack(int x, int y, int i) {
    BMmonsters[i].BM.setTextureRect(BMgetRect(7 + BMmovmentCounter[i]));
    int initial = BMmovmentCounter[i];
    BMupdateMonBMterAnimationCounter(i);
    if (BMmovmentCounter[i] == 4) {
        if (abs(x) < 50 && abs(y) < 30) {
            Player_Health--;
            ishit = true;
        }
        BMmovmentCounter[i] = 0;
        BMtate[i] = BMenum::BM_walk;
    }
}

void BMhurt(int i) {
    BMmonsters[i].BM.setTextureRect(BMgetRect(28 - BMmovmentCounter[i]));
    BMupdateMonBMterAnimationCounter(i);
    if (BMmovmentCounter[i] == 7) {
        BMmovmentCounter[i] = 0;
        BMtate[i] = BMenum::BM_walk;
    }
}

void BMdie(int x, int y,int i) {
    BMmonsters[i].BM.setTextureRect(BMgetRect(21 + BMmovmentCounter[i]));
    BMupdateMonBMterAnimationCounter(i);
    if (BMmovmentCounter[i] == 7) {
        if (abs(x) < 50 && abs(y) < 30) {
            ishit = true;
            Player_Health--;
        }
        BMmonsters[i].alive = false;
        AblazeCharge++;
        coinsCount += 0.2;
    }
}

void BMcreate() {
    BMtexture.loadFromFile("enemies/Brain Mole Monarch.png");
    BMoriginal.health = 1;
    BMoriginal.BM.setTexture(BMtexture);
    BMoriginal.BM.setTextureRect(BMgetRect(0));
    BMoriginal.BM.setOrigin(16, 16);
    BMoriginal.BM.setScale(5, 5);
}

void BMset(int BMn) {
    for (int i = 0; i < BrainMoleNumber; i++) {
        BMmonsters[i] = BMoriginal;
        BMmonBMterCounter[i] = 0;
        BMmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        BMmonsters[i].BM.setPosition(x,y);
        BMmonsters[i].alive = true;
        BMtate[i] = BMenum::BM_hurt;
    }
}

void BMmove(float time, Sprite p, int attct, int& PlayerHealth) {
    BMdeltatime = time;
    for (int i = 0; i < BrainMoleNumber; i++) {
        // check if alive
        if (!BMmonsters[i].alive)
            continue;

        room_cleared = false;
        double x = p.getPosition().x - BMmonsters[i].BM.getPosition().x, y = BMmonsters[i].BM.getPosition().y - p.getPosition().y;

        if (x < 0)
            BMmonsters[i].BM.setScale(Vector2f(-3, 3));
        else
            BMmonsters[i].BM.setScale(Vector2f(3, 3));

        // check if BM is spawning
        if (BMtate[i] == BMenum::BM_hurt) {
            BMhurt(i);
            continue;
        }

        // check if BM iBM being attacked
        if (BMtate[i] != BMenum::BM_die && abs(x) < 100 && abs(y) < 100 && attct) {
            BMmonsters[i].health -= attct;
            BMmovmentCounter[i] = 0;
            BMtate[i] = BMenum::BM_die;
        }

        // check if doing BMomething
        if (BMtate[i] == BMenum::BM_die) {
            BMdie(x,y,i);
            continue;
        }
        else if (BMtate[i] == BMenum::BM_attack) {
            BMattack(x, y, i);
            continue;
        }

        // make deciBMion
        if (abs(x) < 50 && abs(y) < 30) {
            BMmovmentCounter[i] = 0;
            BMtate[i] = BMenum::BM_attack;
            BMattack(x, y, i);
        }
        else
            BMwalk(i);
    }
}

void BMdraw(RenderWindow& window) {
    for (int i = 0; i < BrainMoleNumber; i++)
        if (BMmonsters[i].alive)
        {
            window.draw(BMmonsters[i].BM);
            gui.DrawMonsterHP(BMmonsters[i].BM.getPosition(), BMmonsters[i].health, 1,30, 35);
            
        }
}

