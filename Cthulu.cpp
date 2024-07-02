#include "cthulu.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture Ctexture;
Cthulu Coriginal;
int CmovementCounter[30];
float  CmonsterCounter[30], Cdeltatime;

enum Cenum {
    C_walk, C_attack, C_hurt, C_die, C_spawn
};
Cenum Cstate[30];

IntRect CgetRect(int pos) {
    int x = pos % 15;
    int y = pos / 15;
    return IntRect(x * 192, y * 112, 192, 112);
}


void CupdateMonsterAnimationCounter(int i, float st = 0.15) {
    CmonsterCounter[i] += Cdeltatime;
    if (CmonsterCounter[i] >= st)
    {
        CmonsterCounter[i] = 0;
        CmovementCounter[i]++;
    }
}


void Cwalk(int i) {
    CmovementCounter[i] %= 12;
    Cmonsters[i].Ct.setTextureRect(CgetRect(15 + CmovementCounter[i]));
    Vector2f Direction = Player.getPosition() - Cmonsters[i].Ct.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    Cmonsters[i].Ct.move(Vector2f(Direction.x / (float)(abs(Direction.x) + abs(Direction.y)) * Cmonsters[i].speed * Cdeltatime,
                              Direction.y / (float)(abs(Direction.x) + abs(Direction.y)) * Cmonsters[i].speed * Cdeltatime));
    CupdateMonsterAnimationCounter(i);
    CmovementCounter[i] %= 12;
}

void Cattack(int x, int y, int i) {
    Cmonsters[i].Ct.setTextureRect(CgetRect(45 + CmovementCounter[i]));
    int initial = CmovementCounter[i];
    CupdateMonsterAnimationCounter(i, 0.1);
    if ((CmovementCounter[i] == 3 && CmovementCounter[i] > initial)) {
        if (abs(x) < 200 && abs(y) < 100) {
            Player_Health -= Cmonsters[i].damage;
            ishit = true;
        }
    }
    if (CmovementCounter[i] == 7) {
        CmovementCounter[i] = 0;
        Cmonsters[i].cooldown = 0.3;
        Cstate[i] = Cenum::C_walk;
    }
}


void Churt(int i) {
    Cmonsters[i].Ct.setTextureRect(CgetRect(75 + CmovementCounter[i]));
    CupdateMonsterAnimationCounter(i, 0.1);
    if (CmovementCounter[i] == 5) {
        CmovementCounter[i] = 0;
        AblazeCharge += 4;
        Cstate[i] = Cenum::C_walk;
    }
}


void Cdie(int i) {
    Cmonsters[i].Ct.setTextureRect(CgetRect( 90 + CmovementCounter[i]));
    CupdateMonsterAnimationCounter(i, 0.2);
    if (CmovementCounter[i] == 11)
        Cmonsters[i].alive = false, coinsCount += 30;
}


void Ccreate() {
    Ctexture.loadFromFile("enemies2/cthulu.png");
    Coriginal.health = 150;
    Coriginal.Ct.setTexture(Ctexture);
    Coriginal.Ct.setTextureRect(CgetRect(0));
    Coriginal.Ct.setOrigin(96, 56);
    Coriginal.Ct.setScale(3.5, 3.5);
    Coriginal.Ct.setPosition(300, 6700);
}

void Cset(int Rn) {
    for (int i = 0; i < CthuluNumber; i++) {
        Cmonsters[i] = Coriginal;
        CmonsterCounter[i] = 0;
        CmovementCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        Cmonsters[i].Ct.setPosition(x, y);
        Cmonsters[i].alive = true;
        Cstate[i] = Cenum::C_spawn;
    }
}


void Ctmove(float time, Sprite p, int attct, int& PlayerHealth) {
    Cdeltatime = time;
    for (int i = 0; i < CthuluNumber; i++) {

        Cmonsters[i].cooldown -= Cdeltatime;
        // check if alive
        if (!Cmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if C will die
        if (Cmonsters[i].health <= 0 && Cstate[i] != Cenum::C_die) {
            Cmonsters[i].Ct.setTexture(Ctexture);
            CmovementCounter[i] = 0;
            Cstate[i] = Cenum::C_die;
        }

        double x = p.getPosition().x - Cmonsters[i].Ct.getPosition().x, y = Cmonsters[i].Ct.getPosition().y - p.getPosition().y;

        // check if C is dying
        if (Cstate[i] == Cenum::C_die) {
            Cdie(i);
            continue;
        }

        if (x < 0)
            Cmonsters[i].Ct.setScale(Vector2f(-3.5, 3.5));
        else
            Cmonsters[i].Ct.setScale(Vector2f(3.5, 3.5));

        // check if R is being attacked
        if (Cstate[i] != Cenum::C_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            Cmonsters[i].health -= attct;
            CmovementCounter[i] = 0;
            Cstate[i] = Cenum::C_hurt;
        }

        // check if doing something
        if (Cstate[i] == Cenum::C_hurt) {
            Churt(i);
            continue;
        }
        else if (Cstate[i] == Cenum::C_attack && Cmonsters[i].cooldown <= 0) {
            Cattack(x, y, i);
            continue;
        }

        // make decision
        else if (abs(x) < 200 && abs(y) < 100) {
            Cstate[i] = Cenum::C_attack;
            CmovementCounter[i] = 0;
            Cattack(x, y, i);
        }

        else Cwalk(i);
    }
}

void Cdraw(RenderWindow& window) {
    for (int i = 0; i < CthuluNumber; i++) {
        if (Cmonsters[i].alive) {
            window.draw(Cmonsters[i].Ct);
            gui.drawBossHP(Cmonsters[i].health, 150);
        }
    }
}

