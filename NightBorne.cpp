#include "NightBorne.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture NBtexture;
NightBrone NBoriginal;
int NBmovmentCounter[30];
float NBmonsterCounter[30], NBdeltatime;

enum NBenum {
    NB_walk, NB_attack, NB_hurt, NB_die, NB_spawn
};
NBenum NBstate[30];

IntRect NBgetRect(int pos) {
    int x = pos % 23;
    int y = pos / 23;
    return IntRect(x * 80, y * 80, 80, 80);
}

void NBupdateMonsterAnimationCounter(int i, float st = 0.15) {
    NBmonsterCounter[i] += NBdeltatime;
    if (NBmonsterCounter[i] >= st)
    {
        NBmonsterCounter[i] = 0;
        NBmovmentCounter[i]++;
    }
}

void NBwalk(int i) {
    NBmonsters[i].NB.setTextureRect(NBgetRect(23 + NBmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - NBmonsters[i].NB.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    NBmonsters[i].NB.move(Vector2f(norm_direction.x * NBmonsters[i].speed * NBdeltatime, norm_direction.y * NBmonsters[i].speed * NBdeltatime));
    CheckMonsterCollisions(NBmonsters[i].NB, abs(norm_direction.x * NBmonsters[i].speed * NBdeltatime), abs(norm_direction.y * NBmonsters[i].speed * NBdeltatime));
    NBupdateMonsterAnimationCounter(i);
    NBmovmentCounter[i] %= 6;
}

void NBattack(int x, int y, int i) {
    NBmonsters[i].NB.setTextureRect(NBgetRect(46 + NBmovmentCounter[i]));
    int initial = NBmovmentCounter[i];
    NBupdateMonsterAnimationCounter(i,NBmonsters[i].AttackSpeed);
    if (NBmovmentCounter[i] == 9 && NBmovmentCounter[i] > initial) {
        if (abs(x) < 100 && abs(y) < 30) {
            Player_Health -= NBmonsters[i].damage;
            ishit = true;
        }
    }
    if (NBmovmentCounter[i] == 12) {
        NBmovmentCounter[i] = 0;
        NBstate[i] = NBenum::NB_walk;
    }
}

void NBhurt(int i) {
    NBmonsters[i].NB.setTextureRect(NBgetRect(69 + NBmovmentCounter[i]));
    NBupdateMonsterAnimationCounter(i);
    if (NBmovmentCounter[i] == 5) {
        NBmovmentCounter[i] = 0;
        NBstate[i] = NBenum::NB_walk;
    }
}

void NBdie(int i) {
    
    NBmonsters[i].NB.setTextureRect(NBgetRect(92 + NBmovmentCounter[i]));
    NBupdateMonsterAnimationCounter(i);
    if (NBmovmentCounter[i] == 23) {
        NBmonsters[i].alive = false;
        if (level > 1) AblazeCharge+=2,coinsCount+=0.5;
        else AblazeCharge += 10, coinsCount+=20;
    }
}

void NBspawn(int i) {
    NBmonsters[i].NB.setTextureRect(NBgetRect(NBmovmentCounter[i]));
    NBupdateMonsterAnimationCounter(i);
    if (NBmovmentCounter[i] == 9) {
        NBmovmentCounter[i] = 0;
        NBmonsters[i].AttackSpeed = 0.05;
        NBmonsters[i].speed = 200;
        NBstate[i] = NBenum::NB_walk;
        NBmonsters[i].cooldown = 10;
    }
}

void NBcreate() {
    NBtexture.loadFromFile("enemies/NightBorne.png");
    NBoriginal.health = 7;
    NBoriginal.NB.setTexture(NBtexture);
    NBoriginal.NB.setTextureRect(NBgetRect(0));
    NBoriginal.NB.setOrigin(40, 40);
    NBoriginal.NB.setScale(3, 3);
}

void NBset(int NBn) {
    for (int i = 0; i < NightBroneNumber; i++) {
        NBmonsters[i] = NBoriginal;
        NBmonsterCounter[i] = 0;
        NBmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        NBmonsters[i].NB.setPosition(x,y);
        NBmonsters[i].alive = true;
        NBstate[i] = NBenum::NB_spawn;
        if (level == 1)
            NBmonsters[i].health = 60;
    }
}

void NBmove(float time, Sprite p, int attct, int& PlayerHealth, bool& IsHit) {
    NBdeltatime = time;
    for (int i = 0; i < NightBroneNumber; i++) {

        // check if alive
        if (!NBmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if NB will die
        if (NBmonsters[i].health <= 0 && NBstate[i] != NBenum::NB_die) {
            NBmovmentCounter[i] = 0;
            NBstate[i] = NBenum::NB_die;
        }

        double x = p.getPosition().x - NBmonsters[i].NB.getPosition().x, y = NBmonsters[i].NB.getPosition().y - p.getPosition().y;

        // check if NB is dying
        if (NBstate[i] == NBenum::NB_die) {
            int initial = NBmovmentCounter[i];
            NBdie(i);
            if (NBmovmentCounter[i] == 13 && NBmovmentCounter[i] > initial && abs(x) < 100 && abs(y) < 100){
                IsHit = true;
                PlayerHealth -= 30;
            }
            continue;
        }

        if(level != 1){
            if (x < 0)
                NBmonsters[i].NB.setScale(Vector2f(-3, 3));
            else
                NBmonsters[i].NB.setScale(Vector2f(3, 3));
        }
        else {
            if (x < 0)
                NBmonsters[i].NB.setScale(Vector2f(-5, 5));
            else
                NBmonsters[i].NB.setScale(Vector2f(5, 5));
        }

        NBmonsters[i].cooldown -= NBdeltatime;

        if (NBmonsters[i].cooldown < 7 && level != 1) {
            NBmonsters[i].AttackSpeed = 0.15;
            NBmonsters[i].speed = 100;
        }
        
        // check if NB is spawning
        if (NBstate[i] == NBenum::NB_spawn) {
            NBspawn(i);
            continue;
        }

        // check if NB is being attacked
        if (NBstate[i] != NBenum::NB_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            NBmonsters[i].health -= attct;
            NBmovmentCounter[i] = 0;
            NBstate[i] = NBenum::NB_hurt;
        }

        // check if doing something
        if (NBstate[i] == NBenum::NB_hurt) {
            NBhurt(i);
            continue;
        }
        else if (NBstate[i] == NBenum::NB_attack) {
            NBattack(x, y, i);
            continue;
        }

        // make decision
        if (NBmonsters[i].cooldown <= 0) {
            NBspawn(i);
            NBstate[i] = NBenum::NB_spawn;
        }
        else if (abs(x) < 100 && abs(y) < 30) {
            NBstate[i] = NBenum::NB_attack;
            NBattack(x, y, i);
        }
        
        else NBwalk(i);                                                                        
    }
}

void NBdraw(RenderWindow& window) {
    for (int i = 0; i < NightBroneNumber; i++) {
        if (NBmonsters[i].alive) {
            window.draw(NBmonsters[i].NB);
            if (level == 1)
                gui.drawBossHP(NBmonsters[i].health, 60);
            else
                gui.DrawMonsterHP(NBmonsters[i].NB.getPosition(), NBmonsters[i].health, 7, 20, 15);
        }
    }
}

