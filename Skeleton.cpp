#include "Skeleton.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture Stexture;
Skeleton Soriginal;
int SmovmentCounter[100];
float SmonsterCounter[100], Sdeltatime;

enum Senum {
    S_walk, S_attack, S_hurt, S_die
};
Senum Sstate[100];

IntRect SgetRect(int pos) {
    int x = pos % 13;
    int y = pos / 13;
    return IntRect(x * 64, y * 64, 64, 64);
}

void SupdateMonsterAnimationCounter(int i, float st = 0.15) {
    SmonsterCounter[i] += Sdeltatime;
    if (SmonsterCounter[i] >= st)
    {
        SmonsterCounter[i] = 0;
        SmovmentCounter[i]++;
    }
}

void Swalk(int i) {
    Smonsters[i].S.setTextureRect(SgetRect(26 + SmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - Smonsters[i].S.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    Smonsters[i].S.move(Vector2f(norm_direction.x * Smonsters[i].speed * Sdeltatime, norm_direction.y * Smonsters[i].speed * Sdeltatime));
    CheckMonsterCollisions(Smonsters[i].S, norm_direction.x * Smonsters[i].speed * Sdeltatime, norm_direction.y * Smonsters[i].speed * Sdeltatime);
    SupdateMonsterAnimationCounter(i);
    SmovmentCounter[i] %= 12;
}

void Sattack(int x, int y, int i) {
    Smonsters[i].S.setTextureRect(SgetRect(SmovmentCounter[i]));
    int initial = SmovmentCounter[i];
    SupdateMonsterAnimationCounter(i);
    if ((SmovmentCounter[i] == 4 || SmovmentCounter[i] == 8) && SmovmentCounter[i] > initial) {
        if (abs(x) < 100 && abs(y) < 30) {
            Player_Health--;
            ishit = true;
        }
    }
    if (SmovmentCounter[i] == 13) {
        SmovmentCounter[i] = 0;
        Sstate[i] = Senum::S_walk;
    }
}

void Shurt(int i) {
    Smonsters[i].S.setTextureRect(SgetRect(52 + SmovmentCounter[i]));
    SupdateMonsterAnimationCounter(i);
    if (SmovmentCounter[i] == 3) {
        SmovmentCounter[i] = 0;
        Sstate[i] = Senum::S_walk;
    }
}

void Sdie(int i) {
    Smonsters[i].S.setTextureRect(SgetRect(13 + SmovmentCounter[i]));
    SupdateMonsterAnimationCounter(i);
    if (SmovmentCounter[i] == 13)
        Smonsters[i].sleep = false, coinsCount+=0.1;
}

void Screate() {
    Stexture.loadFromFile("enemies/Skeleton enemy.png");
    Soriginal.health = 5;
    Soriginal.S.setTexture(Stexture);
    Soriginal.S.setTextureRect(SgetRect(26));
    Soriginal.S.setOrigin(32, 32);
    Soriginal.S.setScale(1.5, 1.5);
    Soriginal.sleep = true;
}

void Sset(int Sn) {
    for (int i = 0; i < SkeletonNumber; i++) {
        Smonsters[i] = Soriginal;
        SmonsterCounter[i] = 0;
        SmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        Smonsters[i].S.setPosition(x,y);
        Smonsters[i].alive = true;
        Sstate[i] = Senum::S_hurt;
    }
}

void Smove(float time, Sprite p, int attct, int& PlayerHealth, bool& IsHit) {
    Sdeltatime = time;
    for (int i = 0; i < SkeletonNumber; i++) {

        // check if alive
        if (!Smonsters[i].alive || !Smonsters[i].sleep)
            continue;

        // check if S will die
        if (Smonsters[i].health <= 0 && Sstate[i] != Senum::S_die) {
            SmovmentCounter[i] = 0;
            Sstate[i] = Senum::S_die;
        }

        double x = p.getPosition().x - Smonsters[i].S.getPosition().x, y = Smonsters[i].S.getPosition().y - p.getPosition().y;

        // check if S is dying
        if (Sstate[i] == Senum::S_die) {
            Sdie(i);
            continue;
        }

        if(Smonsters[i].sleep)
            room_cleared = false;

        if (x < 0)
            Smonsters[i].S.setScale(Vector2f(-1.5, 1.5));
        else
            Smonsters[i].S.setScale(Vector2f(1.5, 1.5));

        // check if S is being attacked
        if (Sstate[i] != Senum::S_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            Smonsters[i].health -= attct;
            SmovmentCounter[i] = 0;
            Sstate[i] = Senum::S_hurt;
        }

        // check if doing something
        if (Sstate[i] == Senum::S_hurt) {
            Shurt(i);
            continue;
        }
        else if (Sstate[i] == Senum::S_attack) {
            Sattack(x, y, i);
            continue;
        }

        // make decision
        if (abs(x) < 100 && abs(y) < 30) {
            Sstate[i] = Senum::S_attack;
            Sattack(x, y, i);
        }
        else 
            Swalk(i);
    }
}

void Sdraw(RenderWindow& window) {
    for (int i = 0; i < SkeletonNumber; i++)
        if (Smonsters[i].alive)
        {
            window.draw(Smonsters[i].S);
            gui.DrawMonsterHP(Smonsters[i].S.getPosition(), Smonsters[i].health, 5, 20, 30);
        }
}

void NewSkeleton(Vector2f x) {
    for (int i = 0; i < SkeletonNumber; i++)
        if (!Smonsters[i].alive) {
            Smonsters[i] = Soriginal;
            SmonsterCounter[i] = 0;
            SmovmentCounter[i] = 0;
            Smonsters[i].S.setPosition(x);
            Smonsters[i].alive = true;
            Sstate[i] = Senum::S_hurt;
            return;
        }
    if (SkeletonNumber >= 100) return;
    int i = SkeletonNumber;
    Smonsters[i] = Soriginal;
    SmonsterCounter[i] = 0;
    SmovmentCounter[i] = 0;
    Smonsters[i].S.setPosition(x);
    Smonsters[i].alive = true;
    Sstate[i] = Senum::S_hurt;
    SkeletonNumber++;
}

float dis(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

Vector2f toskel(Vector2f R) {
    for (int i = 0; i < SkeletonNumber; i++) {
        if (!Smonsters[i].sleep) {
            if (dis(R, Smonsters[i].S.getPosition()) < 30) {
                Smonsters[i].sleep = true;
                Smonsters[i].alive = false;
            }
            else
                return Smonsters[i].S.getPosition();
        }
    }
    return Player.getPosition();
}