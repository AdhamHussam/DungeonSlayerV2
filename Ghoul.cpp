#include "Ghoul.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture Gtexture;
Ghoul Goriginal;
int GmovmentCounter[30];
float GmonsterCounter[30], Gdeltatime;

enum Genum {
    G_walk, G_attack, G_hurt, G_die
};
Genum Gtate[30];

IntRect GgetRect(int poG) {
    int x = poG % 8;
    int y = poG / 8;
    return IntRect(x * 32, y * 32, 32, 32);
}

void GupdatemonsterAnimationCounter(int i, float Gt = 0.15) {
    GmonsterCounter[i] += Gdeltatime;
    if (GmonsterCounter[i] >= Gt)
    {
        GmonsterCounter[i] = 0;
        GmovmentCounter[i]++;
    }
}

void Gwalk(int i) {
    GmovmentCounter[i] %= 7;
    Gmonsters[i].G.setTextureRect(GgetRect(8 +GmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - Gmonsters[i].G.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    Gmonsters[i].G.move(Vector2f(norm_direction.x * Gmonsters[i].speed * Gdeltatime, norm_direction.y * Gmonsters[i].speed * Gdeltatime));
    CheckMonsterCollisions(Gmonsters[i].G, norm_direction.x * Gmonsters[i].speed * Gdeltatime, norm_direction.y * Gmonsters[i].speed * Gdeltatime);
    GupdatemonsterAnimationCounter(i);
    GmovmentCounter[i] %= 7;
}

void Gattack(int x, int y, int i) {
    Gmonsters[i].G.setTextureRect(GgetRect( 16 + GmovmentCounter[i]));
    int initial = GmovmentCounter[i];
    GupdatemonsterAnimationCounter(i);
    if (GmovmentCounter[i] == 4 && GmovmentCounter[i] > initial) {
        if (abs(x) < 75 && abs(y) < 30) {
            Player_Health -= 5;
            ishit = true;
        }
    }
    if (GmovmentCounter[i] == 5) {
        GmovmentCounter[i] = 0;
        Gtate[i] = Genum::G_walk;
    }
}

void Ghurt(int i) {
    Gmonsters[i].G.setTextureRect(GgetRect(24 + GmovmentCounter[i]));
    GupdatemonsterAnimationCounter(i);
    if (GmovmentCounter[i] == 3) {
        GmovmentCounter[i] = 0;
        Gtate[i] = Genum::G_walk;
    }
}

void Gdie(int i) {

    Gmonsters[i].G.setTextureRect(GgetRect(32 + GmovmentCounter[i]));
    GupdatemonsterAnimationCounter(i);
    if (GmovmentCounter[i] == 6)
        Gmonsters[i].alive = false, AblazeCharge += 2, coinsCount += 0.5;
}

void Gcreate() {
    Gtexture.loadFromFile("enemies2/Ghoul.png");
    Goriginal.health = 10;
    Goriginal.G.setTexture(Gtexture);
    Goriginal.G.setTextureRect(GgetRect(0));
    Goriginal.G.setOrigin(16, 16);
    Goriginal.G.setScale(3, 3);
}

void Gset(int Gn) {
    for (int i = 0; i < GhoulNumber; i++) {
        Gmonsters[i] = Goriginal;
        GmonsterCounter[i] = 0;
        GmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        Gmonsters[i].G.setPosition(x, y);
        Gmonsters[i].alive = true;
        Gtate[i] = Genum::G_hurt;
    }
}

void Gmove(float time, Sprite p, int attct, int& PlayerHealth) {
    Gdeltatime = time;
    for (int i = 0; i < GhoulNumber; i++) {
        // check if alive
        if (!Gmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if G will die
        if (Gmonsters[i].health <= 0 && Gtate[i] != Genum::G_die) {
            GmovmentCounter[i] = 0;
            Gtate[i] = Genum::G_die;
        }

        double x = p.getPosition().x - Gmonsters[i].G.getPosition().x, y = Gmonsters[i].G.getPosition().y - p.getPosition().y;

        // check if G iG dying
        if (Gtate[i] == Genum::G_die) {
            Gdie(i);
            continue;
        }

        if (x < 0)
            Gmonsters[i].G.setScale(Vector2f(-3, 3));
        else
            Gmonsters[i].G.setScale(Vector2f(3, 3));

        // check if G iG being attacked
        if (Gtate[i] != Genum::G_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            Gmonsters[i].health -= attct;
            GmovmentCounter[i] = 0;
            Gtate[i] = Genum::G_hurt;
        }

        // check if doing Gomething
        if (Gtate[i] == Genum::G_hurt) {
            Ghurt(i);
            continue;
        }
        else if (Gtate[i] == Genum::G_attack) {
            Gattack(x, y, i);
            continue;
        }

        // make deciGion
        if (abs(x) < 75 && abs(y) < 30) {
            GmovmentCounter[i] = 0;
            Gtate[i] = Genum::G_attack;
            Gattack(x, y, i);
        }
        else
            Gwalk(i);
    }
}

void Gdraw(RenderWindow& window) {
    for (int i = 0; i < GhoulNumber; i++)
        if (Gmonsters[i].alive)
        {
            window.draw(Gmonsters[i].G);
            gui.DrawMonsterHP(Gmonsters[i].G.getPosition(), Gmonsters[i].health, 10, 30, 35);
        }
}

