#include "FlameWizard.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture FWtexture;
FlameWizard FWoriginal;
int FWmovmentCounter[30];
float FWmonFWterCounter[30], FWdeltatime;

enum FWenum {
    FW_walk, FW_attack, FW_hurt, FW_die
};
FWenum FWtate[30];

IntRect FWgetRect(int poFW) {
    int x = poFW % 8;
    int y = poFW / 8;
    return IntRect(x * 150, y * 150, 150, 150);
}

void FWupdateMonFWterAnimationCounter(int i, float FWt = 0.15) {
    FWmonFWterCounter[i] += FWdeltatime;
    if (FWmonFWterCounter[i] >= FWt)
    {
        FWmonFWterCounter[i] = 0;
        FWmovmentCounter[i]++;
    }
}

void FWwalk(int i) {
    FWmovmentCounter[i] %= 8;
    FWmonsters[i].FW.setTextureRect(FWgetRect(8 + FWmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - FWmonsters[i].FW.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    FWmonsters[i].FW.move(Vector2f(norm_direction.x * FWmonsters[i].speed * FWdeltatime, norm_direction.y * FWmonsters[i].speed * FWdeltatime));
    //CheckMonsterCollisions(FWmonsters[i].FW, norm_direction.x * FWmonsters[i].speed * FWdeltatime, norm_direction.y * FWmonsters[i].speed * FWdeltatime);
    FWupdateMonFWterAnimationCounter(i);
    FWmovmentCounter[i] %= 8;
}

void FWattack(int x, int y, int i) {
    FWmonsters[i].FW.setTextureRect(FWgetRect(16 + FWmovmentCounter[i]));
    int initial = FWmovmentCounter[i];
    FWupdateMonFWterAnimationCounter(i);
    if (FWmovmentCounter[i] == 3 && FWmovmentCounter[i] > initial) {
        if (abs(x) < 150 && abs(y) < 30) {
            Player_Health -= 5;
            ishit = true;
        }
    }
    if (FWmovmentCounter[i] == 7) {
        FWmovmentCounter[i] = 0;
        FWtate[i] = FWenum::FW_walk;
    }
}

void FWhurt(int i) {
    FWmonsters[i].FW.setTextureRect(FWgetRect(24 + FWmovmentCounter[i]));
    FWupdateMonFWterAnimationCounter(i);
    if (FWmovmentCounter[i] == 4) {
        FWmovmentCounter[i] = 0;
        FWtate[i] = FWenum::FW_walk;
    }
}

void FWdie(int i) {

    FWmonsters[i].FW.setTextureRect(FWgetRect(32 + FWmovmentCounter[i]));
    FWupdateMonFWterAnimationCounter(i);
    if (FWmovmentCounter[i] == 4)
        FWmonsters[i].alive = false, AblazeCharge += 2, coinsCount += 0.5;
}

void FWcreate() {
    FWtexture.loadFromFile("enemies2/Flame_Wizard.png");
    FWoriginal.health = 15;
    FWoriginal.FW.setTexture(FWtexture);
    FWoriginal.FW.setTextureRect(FWgetRect(0));
    FWoriginal.FW.setOrigin(75, 75);
    FWoriginal.FW.setScale(3, 3);
}

void FWset(int FWn) {
    for (int i = 0; i < FlameWizardNumber; i++) {
        FWmonsters[i] = FWoriginal;
        FWmonFWterCounter[i] = 0;
        FWmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        FWmonsters[i].FW.setPosition(x, y);
        FWmonsters[i].alive = true;
        FWtate[i] = FWenum::FW_hurt;
    }
}

void FWmove(float time, Sprite p, int attct, int& PlayerHealth) {
    FWdeltatime = time;
    for (int i = 0; i < FlameWizardNumber; i++) {
        // check if alive
        if (!FWmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if FW will die
        if (FWmonsters[i].health <= 0 && FWtate[i] != FWenum::FW_die) {
            FWmovmentCounter[i] = 0;
            FWtate[i] = FWenum::FW_die;
        }

        double x = p.getPosition().x - FWmonsters[i].FW.getPosition().x, y = FWmonsters[i].FW.getPosition().y - p.getPosition().y;

        // check if FW iFW dying
        if (FWtate[i] == FWenum::FW_die) {
            FWdie(i);
            continue;
        }

        if (x < 0)
            FWmonsters[i].FW.setScale(Vector2f(-3, 3));
        else
            FWmonsters[i].FW.setScale(Vector2f(3, 3));

        // check if FW iFW being attacked
        if (FWtate[i] != FWenum::FW_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            FWmonsters[i].health -= attct;
            FWmovmentCounter[i] = 0;
            FWtate[i] = FWenum::FW_hurt;
        }

        // check if doing FWomething
        if (FWtate[i] == FWenum::FW_hurt) {
            FWhurt(i);
            continue;
        }
        else if (FWtate[i] == FWenum::FW_attack) {
            FWattack(x, y, i);
            continue;
        }

        // make deciFWion
        if (abs(x) < 150 && abs(y) < 30) {
            FWmovmentCounter[i] = 0;
            FWtate[i] = FWenum::FW_attack;
            FWattack(x, y, i);
        }
        else
            FWwalk(i);
    }
}

void FWdraw(RenderWindow& window) {
    for (int i = 0; i < FlameWizardNumber; i++)
        if (FWmonsters[i].alive)
        {
            window.draw(FWmonsters[i].FW);
            gui.DrawMonsterHP(FWmonsters[i].FW.getPosition(), FWmonsters[i].health, 15, 50, 100);
        }
}

