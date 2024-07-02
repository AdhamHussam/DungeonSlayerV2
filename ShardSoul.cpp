#include "ShardSoul.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture SStexture;
ShardSoul SSoriginal;
int SSmovmentCounter[30];
float SSmonSSterCounter[30], SSdeltatime;

enum SSenum {
    SS_walk, SS_attack, SS_hurt, SS_die
};
SSenum SState[30];

IntRect SSgetRect(int poSS) {
    int x = poSS % 8;
    int y = poSS / 8;
    return IntRect(x * 64, y * 64, 64, 64);
}

void SSupdateMonSSterAnimationCounter(int i, float SSt = 0.15) {
    SSmonSSterCounter[i] += SSdeltatime;
    if (SSmonSSterCounter[i] >= SSt)
    {
        SSmonSSterCounter[i] = 0;
        SSmovmentCounter[i]++;
    }
}

void SSwalk(int i) {
    SSmovmentCounter[i] %= 8;
    SSmonsters[i].SS.setTextureRect(SSgetRect(8 + SSmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - SSmonsters[i].SS.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    SSmonsters[i].SS.move(Vector2f(norm_direction.x * SSmonsters[i].speed * SSdeltatime, norm_direction.y * SSmonsters[i].speed * SSdeltatime));
    CheckMonsterCollisions(SSmonsters[i].SS, norm_direction.x * SSmonsters[i].speed * SSdeltatime, norm_direction.y * SSmonsters[i].speed * SSdeltatime);
    SSupdateMonSSterAnimationCounter(i);
    SSmovmentCounter[i] %= 8;
}

void SSattack(int x, int y, int i) {
    SSmonsters[i].SS.setTextureRect(SSgetRect(16 + SSmovmentCounter[i]));
    int initial = SSmovmentCounter[i];
    SSupdateMonSSterAnimationCounter(i);
    if (SSmovmentCounter[i] == 3 && SSmovmentCounter[i] > initial) {
        if (abs(x) < 50 && abs(y) < 30) {
            Player_Health-=2;
            ishit = true;
        }
    }
    if (SSmovmentCounter[i] == 5) {
        SSmovmentCounter[i] = 0;
        SState[i] = SSenum::SS_walk;
    }
}

void SShurt(int i) {
    SSmonsters[i].SS.setTextureRect(SSgetRect(24 + SSmovmentCounter[i]));
    SSupdateMonSSterAnimationCounter(i);
    if (SSmovmentCounter[i] == 4) {
        SSmovmentCounter[i] = 0;
        SState[i] = SSenum::SS_walk;
    }
}

void SSdie(int i) {
    
    SSmonsters[i].SS.setTextureRect(SSgetRect(32 + SSmovmentCounter[i]));
    SSupdateMonSSterAnimationCounter(i);
    if (SSmovmentCounter[i] == 6)
        SSmonsters[i].alive = false, AblazeCharge+=2, coinsCount+=0.5;
}

void SScreate() {
    SStexture.loadFromFile("enemies/Shardsoul Slayer.png");
    SSoriginal.health = 7;
    SSoriginal.SS.setTexture(SStexture);
    SSoriginal.SS.setTextureRect(SSgetRect(0));
    SSoriginal.SS.setOrigin(32, 32);
    SSoriginal.SS.setScale(2.5, 2.5);
}

void SSset(int SSn) {
    for (int i = 0; i < ShardSoulNumber; i++) {
        SSmonsters[i] = SSoriginal;
        SSmonSSterCounter[i] = 0;
        SSmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        SSmonsters[i].SS.setPosition(x,y);
        SSmonsters[i].alive = true;
        SState[i] = SSenum::SS_hurt;
    }
}

void SSmove(float time, Sprite p, int attct, int& PlayerHealth) {
    SSdeltatime = time;
    for (int i = 0; i < ShardSoulNumber; i++) {
        // check if alive
        if (!SSmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if SS will die
        if (SSmonsters[i].health <= 0 && SState[i] != SSenum::SS_die) {
            SSmovmentCounter[i] = 0;
            SState[i] = SSenum::SS_die;
        }

        double x = p.getPosition().x - SSmonsters[i].SS.getPosition().x, y = SSmonsters[i].SS.getPosition().y - p.getPosition().y;

        // check if SS iSS dying
        if (SState[i] == SSenum::SS_die) {
            SSdie(i);
            continue;
        }

        if (x < 0)
            SSmonsters[i].SS.setScale(Vector2f(-3, 3));
        else
            SSmonsters[i].SS.setScale(Vector2f(3, 3));

        // check if SS iSS being attacked
        if (SState[i] != SSenum::SS_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            SSmonsters[i].health -= attct;
            SSmovmentCounter[i] = 0;
            SState[i] = SSenum::SS_hurt;
        }

        // check if doing SSomething
        if (SState[i] == SSenum::SS_hurt) {
            SShurt(i);
            continue;
        }
        else if (SState[i] == SSenum::SS_attack) {
            SSattack(x, y, i);
            continue;
        }

        // make deciSSion
        if (abs(x) < 50 && abs(y) < 30) {
            SSmovmentCounter[i] = 0;
            SState[i] = SSenum::SS_attack;
            SSattack(x, y, i);
        }
        else
            SSwalk(i);
    }
}

void SSdraw(RenderWindow& window) {
    for (int i = 0; i < ShardSoulNumber; i++)
        if (SSmonsters[i].alive)
        {
            window.draw(SSmonsters[i].SS);
            gui.DrawMonsterHP(SSmonsters[i].SS.getPosition(), SSmonsters[i].health, 7, 20, 15);
        }
}

