#include "IceViking.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture IVtexture;
IceViking IVoriginal;
int IVmovmentCounter[30];
float IVmonsterCounter[30], IVdeltatime;

enum IVenum {
    IV_walk, IV_attack, IV_hurt, IV_die
};
IVenum IVtate[30];

IntRect IVgetRect(int poIV) {
    int x = poIV % 12;
    int y = poIV / 12;
    return IntRect(x * 115, y * 84, 115, 84);
}

void IVupdatemonsterAnimationCounter(int i, float Gt = 0.15) {
    IVmonsterCounter[i] += IVdeltatime;
    if (IVmonsterCounter[i] >= Gt)
    {
        IVmonsterCounter[i] = 0;
        IVmovmentCounter[i]++;
    }
}

void IVwalk(int i) {
    IVmovmentCounter[i] %= 7;
    IVmonsters[i].IV.setTextureRect(IVgetRect(12 + IVmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - IVmonsters[i].IV.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    IVmonsters[i].IV.move(Vector2f(norm_direction.x * IVmonsters[i].speed * IVdeltatime, norm_direction.y * IVmonsters[i].speed * IVdeltatime));
    CheckMonsterCollisions(IVmonsters[i].IV, norm_direction.x * IVmonsters[i].speed * IVdeltatime, norm_direction.y * IVmonsters[i].speed * IVdeltatime);
    IVupdatemonsterAnimationCounter(i);
    IVmovmentCounter[i] %= 7;
}

void IVattack(int x, int y, int i) {
    IVmonsters[i].IV.setTextureRect(IVgetRect(240 + IVmovmentCounter[i]));
    int initial = IVmovmentCounter[i];
    IVupdatemonsterAnimationCounter(i);
    if (IVmovmentCounter[i] == 4 && IVmovmentCounter[i] > initial) {
        if (abs(x) < 400 && abs(y) < 30) {
            Player_Health -= 10;
            ishit = true;
        }
    }
    if (IVmovmentCounter[i] == 10) {
        IVmovmentCounter[i] = 0;
        IVtate[i] = IVenum::IV_walk;
    }
}

void IVhurt(int i) {
    IVmonsters[i].IV.setTextureRect(IVgetRect(36 + IVmovmentCounter[i]));
    IVupdatemonsterAnimationCounter(i);
    if (IVmovmentCounter[i] == 3) {
        IVmovmentCounter[i] = 0;
        IVtate[i] = IVenum::IV_walk;
    }
}

void IVdie(int i) {

    IVmonsters[i].IV.setTextureRect(IVgetRect(48 + IVmovmentCounter[i]));
    IVupdatemonsterAnimationCounter(i);
    if (IVmovmentCounter[i] == 11)
        IVmonsters[i].alive = false, AblazeCharge += 2, coinsCount += 0.5;
}

void IVcreate() {
    IVtexture.loadFromFile("enemies2/IceViking.png");
    IVoriginal.health = 15;
    IVoriginal.IV.setTexture(IVtexture);
    IVoriginal.IV.setTextureRect(IVgetRect(0));
    IVoriginal.IV.setOrigin(57, 42);
    IVoriginal.IV.setScale(3, 3);
}

void IVset(int Gn) {
    for (int i = 0; i < IceVikingNumber; i++) {
        IVmonsters[i] = IVoriginal;
        IVmonsterCounter[i] = 0;
        IVmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        IVmonsters[i].IV.setPosition(x, y);
        IVmonsters[i].alive = true;
        IVtate[i] = IVenum::IV_hurt;
    }
}

void IVmove(float time, Sprite p, int attct, int& PlayerHealth) {
    IVdeltatime = time;
    for (int i = 0; i < IceVikingNumber; i++) {
        // check if alive
        if (!IVmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if G will die
        if (IVmonsters[i].health <= 0 && IVtate[i] != IVenum::IV_die) {
            IVmovmentCounter[i] = 0;
            IVtate[i] = IVenum::IV_die;
        }

        double x = p.getPosition().x - IVmonsters[i].IV.getPosition().x, y = IVmonsters[i].IV.getPosition().y - p.getPosition().y;

        // check if G iG dying
        if (IVtate[i] == IVenum::IV_die) {
            IVdie(i);
            continue;
        }

        if (x < 0)
            IVmonsters[i].IV.setScale(Vector2f(-3, 3));
        else
            IVmonsters[i].IV.setScale(Vector2f(3, 3));

        // check if G iG being attacked
        if (IVtate[i] != IVenum::IV_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            IVmonsters[i].health -= attct;
            IVmovmentCounter[i] = 0;
            IVtate[i] = IVenum::IV_hurt;
        }

        // check if doing Gomething
        if (IVtate[i] == IVenum::IV_hurt) {
            IVhurt(i);
            continue;
        }
        else if (IVtate[i] == IVenum::IV_attack) {
            IVattack(x, y, i);
            continue;
        }

        // make deciGion
        if (abs(x) < 400 && abs(y) < 30) {
            IVmovmentCounter[i] = 0;
            IVtate[i] = IVenum::IV_attack;
            IVattack(x, y, i);
        }
        else
            IVwalk(i);
    }
}

void IVdraw(RenderWindow& window) {
    for (int i = 0; i < IceVikingNumber; i++)
        if (IVmonsters[i].alive)
        {
            window.draw(IVmonsters[i].IV);
            gui.DrawMonsterHP(IVmonsters[i].IV.getPosition(), IVmonsters[i].health, 15, 30, 70);
        }
}

