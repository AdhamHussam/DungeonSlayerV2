#include "CrystalMauler.h"
#include "globals.h"
#include "RandomizePlaces.h"

Texture CMtexture;
CrystalMauler CMoriginal;
int CMmovmentCounter[30];
float CMmonsterCounter[30], CMdeltatime;

enum CMenum {
    CM_walk, CM_attack, CM_hurt, CM_die
};
CMenum CMtate[30];

IntRect CMgetRect(int poCM) {
    int x = poCM % 21;
    int y = poCM / 21;
    return IntRect(x * 288, y * 128, 288, 128);
}

void CMupdatemonsterAnimationCounter(int i, float Gt = 0.1) {
    CMmonsterCounter[i] += CMdeltatime;
    if (CMmonsterCounter[i] >= Gt)
    {
        CMmonsterCounter[i] = 0;
        CMmovmentCounter[i]++;
    }
}

void CMwalk(int i) {
    CMmovmentCounter[i] %= 7;
    CMmonsters[i].CM.setTextureRect(CMgetRect(21 + CMmovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - CMmonsters[i].CM.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    CMmonsters[i].CM.move(Vector2f(norm_direction.x * CMmonsters[i].speed * CMdeltatime, norm_direction.y * CMmonsters[i].speed * CMdeltatime));
    CheckMonsterCollisions(CMmonsters[i].CM, norm_direction.x * CMmonsters[i].speed * CMdeltatime, norm_direction.y * CMmonsters[i].speed * CMdeltatime);
    CMupdatemonsterAnimationCounter(i);
    CMmovmentCounter[i] %= 7;
}

void CMattack(int x, int y, int i) {
    CMmonsters[i].CM.setTextureRect(CMgetRect(210 + CMmovmentCounter[i]));
    int initial = CMmovmentCounter[i];
    CMupdatemonsterAnimationCounter(i);
    if (CMmovmentCounter[i] == 7 && CMmovmentCounter[i] > initial) {
        if (abs(x) < 400 && abs(y) < 100) {
            Player_Health -= 10;
            ishit = true;
        }
    }
    if (CMmovmentCounter[i] == 10) {
        CMmovmentCounter[i] = 0;
        CMtate[i] = CMenum::CM_walk;
    }
}

void CMhurt(int i) {
    CMmonsters[i].CM.setTextureRect(CMgetRect(252 + CMmovmentCounter[i]));
    CMupdatemonsterAnimationCounter(i);
    if (CMmovmentCounter[i] == 5) {
        CMmovmentCounter[i] = 0;
        CMtate[i] = CMenum::CM_walk;
    }
}

void CMdie(int i) {

    CMmonsters[i].CM.setTextureRect(CMgetRect(273 + CMmovmentCounter[i]));
    CMupdatemonsterAnimationCounter(i);
    if (CMmovmentCounter[i] == 11)
        CMmonsters[i].alive = false, AblazeCharge += 2, coinsCount += 0.5;
}

void CMcreate() {
    CMtexture.loadFromFile("enemies2/CrystalMauler.png");
    CMoriginal.health = 15;
    CMoriginal.CM.setTexture(CMtexture);
    CMoriginal.CM.setTextureRect(CMgetRect(0));
    CMoriginal.CM.setOrigin(144, 100);
    CMoriginal.CM.setScale(3, 3);
}

void CMset(int Gn) {
    for (int i = 0; i < CrystalMaulerNumber; i++) {
        CMmonsters[i] = CMoriginal;
        CMmonsterCounter[i] = 0;
        CMmovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        CMmonsters[i].CM.setPosition(x, y);
        CMmonsters[i].alive = true;
        CMtate[i] = CMenum::CM_hurt;
    }
}

void CMmove(float time, Sprite p, int attct, int& PlayerHealth) {
    CMdeltatime = time;
    for (int i = 0; i < CrystalMaulerNumber; i++) {
        // check if alCMe
        if (!CMmonsters[i].alive)
            continue;

        room_cleared = false;
        // check if G will die
        if (CMmonsters[i].health <= 0 && CMtate[i] != CMenum::CM_die) {
            CMmovmentCounter[i] = 0;
            CMtate[i] = CMenum::CM_die;
        }

        double x = p.getPosition().x - CMmonsters[i].CM.getPosition().x, y = CMmonsters[i].CM.getPosition().y - p.getPosition().y;

        // check if G iG dying
        if (CMtate[i] == CMenum::CM_die) {
            CMdie(i);
            continue;
        }

        if (x < 0)
            CMmonsters[i].CM.setScale(Vector2f(-3, 3));
        else
            CMmonsters[i].CM.setScale(Vector2f(3, 3));

        // check if G iG being attacked
        if (CMtate[i] != CMenum::CM_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            CMmonsters[i].health -= attct;
            CMmovmentCounter[i] = 0;
            CMtate[i] = CMenum::CM_hurt;
        }

        // check if doing Gomething
        if (CMtate[i] == CMenum::CM_hurt) {
            CMhurt(i);
            continue;
        }
        else if (CMtate[i] == CMenum::CM_attack) {
            CMattack(x, y, i);
            continue;
        }

        // make deciGion
        if (abs(x) < 400 && abs(y) < 100) {
            CMmovmentCounter[i] = 0;
            CMtate[i] = CMenum::CM_attack;
            CMattack(x, y, i);
        }
        else
            CMwalk(i);
    }
}

void CMdraw(RenderWindow& window) {
    for (int i = 0; i < CrystalMaulerNumber; i++)
        if (CMmonsters[i].alive)
        {
            window.draw(CMmonsters[i].CM);
            gui.DrawMonsterHP(CMmonsters[i].CM.getPosition(), CMmonsters[i].health, 15, 30, 40);
        }
}

