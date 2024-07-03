#include "Cacodaemon.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture CDtexture;
Cacodaemon CDoriginal;
int CDmovmentCounter[30];
float CDmonCDterCounter[30], CDdeltatime;

enum CDenum {
    CD_walk, CD_attack, CD_hurt, CD_die
};
CDenum CDtate[30];

IntRect CDgetRect(int poCD) {
    int x = poCD % 8;
    int y = poCD / 8;
    return IntRect(x * 64, y * 64 ,64, 64);
}

void CDupdateMonCDterAnimationCounter(int i, float CDt = 0.15) {
    CDmonCDterCounter[i] += CDdeltatime;
    if (CDmonCDterCounter[i] >= CDt)
    {
        CDmonCDterCounter[i] = 0;
        CDmovmentCounter[i]++;
    }
}

void CDwalk(int i) {
    CDmovmentCounter[i] %= 6;
    CDmonsters[i].CD.setTextureRect(CDgetRect(CDmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - CDmonsters[i].CD.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    CDmonsters[i].CD.move(Vector2f(norm_direction.x * CDmonsters[i].speed * CDdeltatime, norm_direction.y * CDmonsters[i].speed * CDdeltatime));
    //CheckMonsterCollisions(CDmonsters[i].CD, norm_direction.x * CDmonsters[i].speed * CDdeltatime, norm_direction.y * CDmonsters[i].speed * CDdeltatime);
    CDupdateMonCDterAnimationCounter(i);
    CDmovmentCounter[i] %= 6;
}

void CDattack(int x, int y, int i) {
    CDmonsters[i].CD.setTextureRect(CDgetRect(8 + CDmovmentCounter[i]));
    int initial = CDmovmentCounter[i];
    CDupdateMonCDterAnimationCounter(i);
    if (CDmovmentCounter[i] == 4 && CDmovmentCounter[i] > initial) {
        if (abs(x) < 150 && abs(y) < 30) {
            Player_Health -= 5;
            ishit = true;
        }
    }
    if (CDmovmentCounter[i] == 5) {
        CDmovmentCounter[i] = 0;
        CDtate[i] = CDenum::CD_walk;
    }
}

void CDhurt(int i) {
    CDmonsters[i].CD.setTextureRect(CDgetRect(16 + CDmovmentCounter[i]));
    CDupdateMonCDterAnimationCounter(i);
    if (CDmovmentCounter[i] == 3) {
        CDmovmentCounter[i] = 0;
        CDtate[i] = CDenum::CD_walk;
    }
}

void CDdie(int i) {

    CDmonsters[i].CD.setTextureRect(CDgetRect(24 + CDmovmentCounter[i]));
    CDupdateMonCDterAnimationCounter(i);
    if (CDmovmentCounter[i] == 7)
        CDmonsters[i].alive = false, AblazeCharge += 2, coinsCount += 0.5;
}

void CDcreate() {
    CDtexture.loadFromFile("enemies2/Cacodaemon.png");
    CDoriginal.health = 15;
    CDoriginal.CD.setTexture(CDtexture);
    CDoriginal.CD.setTextureRect(CDgetRect(0));
    CDoriginal.CD.setOrigin(32, 32);
    CDoriginal.CD.setScale(3, 3);
}

void CDset(int CDn) {
    for (int i = 0; i < CacodaemonNumber; i++) {
        CDmonsters[i] = CDoriginal;
        CDmonCDterCounter[i] = 0;
        CDmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        CDmonsters[i].CD.setPosition(x, y);
        CDmonsters[i].alive = true;
        CDtate[i] = CDenum::CD_hurt;
    }
}

void CDmove(float time, Sprite p, int attct, int& PlayerHealth) {
    CDdeltatime = time;
    for (int i = 0; i < CacodaemonNumber; i++) {
        // check if alive
        if (!CDmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if CD will die
        if (CDmonsters[i].health <= 0 && CDtate[i] != CDenum::CD_die) {
            CDmovmentCounter[i] = 0;
            CDtate[i] = CDenum::CD_die;
        }

        double x = p.getPosition().x - CDmonsters[i].CD.getPosition().x, y = CDmonsters[i].CD.getPosition().y - p.getPosition().y;

        // check if CD iCD dying
        if (CDtate[i] == CDenum::CD_die) {
            CDdie(i);
            continue;
        }

        if (x < 0)
            CDmonsters[i].CD.setScale(Vector2f(-3, 3));
        else
            CDmonsters[i].CD.setScale(Vector2f(3, 3));

        // check if CD iCD being attacked
        if (CDtate[i] != CDenum::CD_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            CDmonsters[i].health -= attct;
            CDmovmentCounter[i] = 0;
            CDtate[i] = CDenum::CD_hurt;
        }

        // check if doing CDomething
        if (CDtate[i] == CDenum::CD_hurt) {
            CDhurt(i);
            continue;
        }
        else if (CDtate[i] == CDenum::CD_attack) {
            CDattack(x, y, i);
            continue;
        }

        // make deciCDion
        if (abs(x) < 150 && abs(y) < 30) {
            CDmovmentCounter[i] = 0;
            CDtate[i] = CDenum::CD_attack;
            CDattack(x, y, i);
        }
        else
            CDwalk(i);
    }
}

void CDdraw(RenderWindow& window) {
    for (int i = 0; i < CacodaemonNumber; i++)
        if (CDmonsters[i].alive)
        {
            window.draw(CDmonsters[i].CD);
            gui.DrawMonsterHP(CDmonsters[i].CD.getPosition(), CDmonsters[i].health, 15, 20, 50);
        }
}

