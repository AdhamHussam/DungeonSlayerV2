#include "FireKnight.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture FKtexture;
FireKnight FKoriginal;
int FKmovmentCounter[30];
float FKmonFKterCounter[30], FKdeltatime;

enum FKenum {
    FK_walk, FK_attack, FK_hurt, FK_die
};
FKenum FKtate[30];

IntRect FKgetRect(int poFK) {
    int x = poFK % 28;
    int y = poFK / 28;
    return IntRect(x * 288, y * 128, 288, 128);
}

void FKupdateMonFKterAnimationCounter(int i, float FKt = 0.1) {
    FKmonFKterCounter[i] += FKdeltatime;
    if (FKmonFKterCounter[i] >= FKt)
    {
        FKmonFKterCounter[i] = 0;
        FKmovmentCounter[i]++;
    }
}

void FKwalk(int i) {
    FKmovmentCounter[i] %= 8;
    FKmonsters[i].FK.setTextureRect(FKgetRect(28 + FKmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - FKmonsters[i].FK.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    FKmonsters[i].FK.move(Vector2f(norm_direction.x * FKmonsters[i].speed * FKdeltatime, norm_direction.y * FKmonsters[i].speed * FKdeltatime));
    //CheckMonsterCollisions(FKmonsters[i].FK, norm_direction.x * FKmonsters[i].speed * FKdeltatime, norm_direction.y * FKmonsters[i].speed * FKdeltatime);
    FKupdateMonFKterAnimationCounter(i);
    FKmovmentCounter[i] %= 8;
}

void FKattack(int x, int y, int i) {
    FKmonsters[i].FK.setTextureRect(FKgetRect(280 + FKmovmentCounter[i]));
    int initial = FKmovmentCounter[i];
    FKupdateMonFKterAnimationCounter(i);
    if (FKmovmentCounter[i] == 11 && FKmovmentCounter[i] > initial) {
        if (abs(x) < 250 && abs(y) < 30) {
            Player_Health -= 15;
            ishit = true;
        }
    }
    if (FKmovmentCounter[i] == 17) {
        FKmovmentCounter[i] = 0;
        FKtate[i] = FKenum::FK_walk;
    }
}

void FKhurt(int i) {
    FKmonsters[i].FK.setTextureRect(FKgetRect(336 + FKmovmentCounter[i]));
    FKupdateMonFKterAnimationCounter(i);
    if (FKmovmentCounter[i] == 4) {
        FKmovmentCounter[i] = 0;
        FKtate[i] = FKenum::FK_walk;
    }
}

void FKdie(int i) {

    FKmonsters[i].FK.setTextureRect(FKgetRect(364 + FKmovmentCounter[i]));
    FKupdateMonFKterAnimationCounter(i);
    if (FKmovmentCounter[i] == 11)
        FKmonsters[i].alive = false, AblazeCharge += 2, coinsCount += 0.5;
}

void FKcreate() {
    FKtexture.loadFromFile("enemies2/Fire_Knight.png");
    FKoriginal.health = 25;
    FKoriginal.FK.setTexture(FKtexture);
    FKoriginal.FK.setTextureRect(FKgetRect(0));
    FKoriginal.FK.setOrigin(144, 94);
    FKoriginal.FK.setScale(3, 3);
}

void FKset(int FKn) {
    for (int i = 0; i < FireKnightNumber; i++) {
        FKmonsters[i] = FKoriginal;
        FKmonFKterCounter[i] = 0;
        FKmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        FKmonsters[i].FK.setPosition(x, y);
        FKmonsters[i].alive = true;
        FKtate[i] = FKenum::FK_hurt;
    }
}

void FKmove(float time, Sprite p, int attct, int& PlayerHealth) {
    FKdeltatime = time;
    for (int i = 0; i < FireKnightNumber; i++) {
        // check if alive
        if (!FKmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if FK will die
        if (FKmonsters[i].health <= 0 && FKtate[i] != FKenum::FK_die) {
            FKmovmentCounter[i] = 0;
            FKtate[i] = FKenum::FK_die;
        }

        double x = p.getPosition().x - FKmonsters[i].FK.getPosition().x, y = FKmonsters[i].FK.getPosition().y - p.getPosition().y;

        // check if FK iFK dying
        if (FKtate[i] == FKenum::FK_die) {
            FKdie(i);
            continue;
        }

        if (x < 0)
            FKmonsters[i].FK.setScale(Vector2f(-3, 3));
        else
            FKmonsters[i].FK.setScale(Vector2f(3, 3));

        // check if FK iFK being attacked
        if (FKtate[i] != FKenum::FK_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            FKmonsters[i].health -= attct;
            FKmovmentCounter[i] = 0;
            FKtate[i] = FKenum::FK_hurt;
        }

        // check if doing FKomething
        if (FKtate[i] == FKenum::FK_hurt) {
            FKhurt(i);
            continue;
        }
        else if (FKtate[i] == FKenum::FK_attack) {
            FKattack(x, y, i);
            continue;
        }

        // make deciFKion
        if (abs(x) < 250 && abs(y) < 30) {
            FKmovmentCounter[i] = 0;
            FKtate[i] = FKenum::FK_attack;
            FKattack(x, y, i);
        }
        else
            FKwalk(i);
    }
}

void FKdraw(RenderWindow& window) {
    for (int i = 0; i < FireKnightNumber; i++)
        if (FKmonsters[i].alive)
        {
            window.draw(FKmonsters[i].FK);
            gui.DrawMonsterHP(FKmonsters[i].FK.getPosition(), FKmonsters[i].health, 25, 20, 70);
        }
}

