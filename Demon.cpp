#include "Demon.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture Dtexture;
Demon Doriginal;
int DmovementCounter[30];
float DmonsterCounter[30], Ddeltatime;

enum Denum {
    D_walk, D_attack, D_hurt, D_die, D_spawn
};
Denum Dstate[30];

IntRect DgetRect(int pos) {
    int x = pos % 22;
    int y = pos / 22;
    return IntRect(x * 288, y * 160, 288, 160);
}


void DupdateMonsterAnimationCounter(int i, float st = 0.15) {
    DmonsterCounter[i] += Ddeltatime;
    if (DmonsterCounter[i] >= st)
    {
        DmonsterCounter[i] = 0;
        DmovementCounter[i]++;
    }
}


void Dwalk(int i) {
    DmovementCounter[i] %= 12;
    Dmonsters[i].D.setTextureRect(DgetRect(22 + DmovementCounter[i]));
    Vector2f Direction = Player.getPosition() - Dmonsters[i].D.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    Dmonsters[i].D.move(Vector2f(Direction.x / (float)(abs(Direction.x) + abs(Direction.y)) * Dmonsters[i].speed * Ddeltatime,
        Direction.y / (float)(abs(Direction.x) + abs(Direction.y)) * Dmonsters[i].speed * Ddeltatime));
    DupdateMonsterAnimationCounter(i);
    DmovementCounter[i] %= 12;
}

void Dattack(int x, int y, int i) {
    Dmonsters[i].D.setTextureRect(DgetRect(44 + DmovementCounter[i]));
    int initial = DmovementCounter[i];
    DupdateMonsterAnimationCounter(i, 0.1);
    if ((DmovementCounter[i] == 11 && DmovementCounter[i] > initial)) {
        if (abs(x) < 275 && abs(y) < 100) {
            Player_Health -= Dmonsters[i].damage;
            ishit = true;
        }
    }
    if (DmovementCounter[i] == 15) {
        DmovementCounter[i] = 0;
        Dmonsters[i].cooldown = 0.3;
        Dstate[i] = Denum::D_walk;
    }
}


void Dhurt(int i) {
    Dmonsters[i].D.setTextureRect(DgetRect(66 + DmovementCounter[i]));
    DupdateMonsterAnimationCounter(i, 0.1);
    if (DmovementCounter[i] == 4) {
        DmovementCounter[i] = 0;
        AblazeCharge += 4;
        Dstate[i] = Denum::D_walk;
    }
}


void Ddie(int i) {
    Dmonsters[i].D.setTextureRect(DgetRect(88 + DmovementCounter[i]));
    DupdateMonsterAnimationCounter(i, 0.2);
    if (DmovementCounter[i] == 21)
        Dmonsters[i].alive = false, coinsCount += 30;
}


void Dcreate() {
    Dtexture.loadFromFile("enemies2/demon.png");
    Doriginal.health = 400;
    Doriginal.D.setTexture(Dtexture);
    Doriginal.D.setTextureRect(DgetRect(0));
    Doriginal.D.setOrigin(144, 100);
    Doriginal.D.setScale(2.5, 2.5);
    Doriginal.D.setPosition(300, 6700);
}

void Dset(int Rn) {
    for (int i = 0; i < DemonNumber; i++) {
        Dmonsters[i] = Doriginal;
        DmonsterCounter[i] = 0;
        DmovementCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        Dmonsters[i].D.setPosition(x, y);
        Dmonsters[i].alive = true;
        Dstate[i] = Denum::D_spawn;
    }
}


void Dmove(float time, Sprite p, int attct, int& PlayerHealth) {
    Ddeltatime = time;
    for (int i = 0; i < DemonNumber; i++) {

        Dmonsters[i].cooldown -= Ddeltatime;
        // check if alive
        if (!Dmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if C will die
        if (Dmonsters[i].health <= 0 && Dstate[i] != Denum::D_die) {
            Dmonsters[i].D.setTexture(Dtexture);
            DmovementCounter[i] = 0;
            Dstate[i] = Denum::D_die;
        }

        double x = p.getPosition().x - Dmonsters[i].D.getPosition().x, y = Dmonsters[i].D.getPosition().y - p.getPosition().y;

        // check if C is dying
        if (Dstate[i] == Denum::D_die) {
            Ddie(i);
            continue;
        }

        if (x < 0)
            Dmonsters[i].D.setScale(Vector2f(2.5, 2.5));
        else
            Dmonsters[i].D.setScale(Vector2f(-2.5, 2.5));

        // check if R is being attacked
        if (Dstate[i] != Denum::D_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            Dmonsters[i].health -= attct;
            DmovementCounter[i] = 0;
            Dstate[i] = Denum::D_hurt;
        }

        // check if doing something
        if (Dstate[i] == Denum::D_hurt) {
            Dhurt(i);
            continue;
        }
        else if (Dstate[i] == Denum::D_attack && Dmonsters[i].cooldown <= 0) {
            Dattack(x, y, i);
            continue;
        }

        // make decision
        else if (abs(x) < 275 && abs(y) < 100) {
            Dstate[i] = Denum::D_attack;
            DmovementCounter[i] = 0;
            Dattack(x, y, i);
        }

        else Dwalk(i);
    }
}

void Ddraw(RenderWindow& window) {
    for (int i = 0; i < DemonNumber; i++) {
        if (Dmonsters[i].alive) {
            window.draw(Dmonsters[i].D);
            gui.drawBossHP(Dmonsters[i].health, 400);
        }
    }
}

