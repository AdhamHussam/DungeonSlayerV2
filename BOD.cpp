#include "includes.h"
#include "BOD.h"
#include "globals.h"
#include "RandomizePlaces.h"

int MovmentCounter[30];
float MonsterCounter[30];

// function for setting rectangle of texture
IntRect getRect(int pos) {
    int x = pos % 8;
    int y = pos / 8;
    return IntRect(x * 140, y * 93, 140, 93);
}

// updating sprites animation
void UpdateMonsterAnimationCounter(int i, float st = 0.15) {
    MonsterCounter[i] += playerdeltatime;
    if (MonsterCounter[i] >= st)
    {
        MonsterCounter[i] = 0;
        MovmentCounter[i]++;
    }
}

// make monster walk

void Bringerwalk(int x, int y, int i) {
    BODmonsters[i].BOD.setTextureRect(getRect(8 + MovmentCounter[i]));
    Vector2f Direction = Player.getPosition() - BODmonsters[i].BOD.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    BODmonsters[i].BOD.move(Vector2f(norm_direction.x * BODmonsters[i].speed * playerdeltatime, norm_direction.y * BODmonsters[i].speed * playerdeltatime));
    CheckMonsterCollisions(BODmonsters[i].BOD, norm_direction.x * BODmonsters[i].speed * playerdeltatime, norm_direction.y * BODmonsters[i].speed * playerdeltatime);
    //BODmonsters[i].BOD.move(((abs(x) > 1) ? ((x > 0) ? BODmonsters[i].speed*playerdeltatime : -BODmonsters[i].speed* playerdeltatime) : 0), ((y < 0) ? BODmonsters[i].speed* playerdeltatime : -BODmonsters[i].speed* playerdeltatime));
    UpdateMonsterAnimationCounter(i);
    MovmentCounter[i] %= 8;

}


// make monster attack
void attack(int x, int y, int i) {
    BODmonsters[i].BOD.setTextureRect(getRect(16 + MovmentCounter[i]));
    int initial = MovmentCounter[i];
    UpdateMonsterAnimationCounter(i);
    if (MovmentCounter[i] == 5 && MovmentCounter[i] > initial) {
        if (abs(x) < 200 && abs(y) < 30) {
            Player_Health -= 5;
            ishit = true;
        }
    }
    if (MovmentCounter[i] == 10) {
        MovmentCounter[i] = 0;
        BODstate[i] = BOD::BODwalk;
    }
}

// make monster cast his spell
void cast(int i) {
    BODmonsters[i].BOD.setTextureRect(getRect(39 + MovmentCounter[i]));
    BODmonsters[i].spell.setTextureRect(getRect(52 + MovmentCounter[i]));
    UpdateMonsterAnimationCounter(i);
    if (MovmentCounter[i] == 9) {
        MovmentCounter[i] = 0;
        BODstate[i] = BOD::BODwalk;
    }
}

// make monster take damage
void hurt(int i) {
    BODmonsters[i].BOD.setTextureRect(getRect(27 + ((MovmentCounter[i] > 4) ? 8 - MovmentCounter[i] : MovmentCounter[i])));
    UpdateMonsterAnimationCounter(i);
    if (MovmentCounter[i] == 9) {
        MovmentCounter[i] = 0;
        BODstate[i] = BOD::BODwalk;
    }
}

// make monster die
void die(int i) {

    BODmonsters[i].BOD.setTextureRect(getRect(29 + MovmentCounter[i]));
    UpdateMonsterAnimationCounter(i);
    if (MovmentCounter[i] == 10)
    {
        BODalive[i] = false, AblazeCharge += 2;
        coinsCount += 0.5;
    }
}

// make monster spawn
void spawn(int i) {
    BODmonsters[i].BOD.setTextureRect(getRect(29 + 9 - MovmentCounter[i]));
    UpdateMonsterAnimationCounter(i);
    if (MovmentCounter[i] == 10) {
        MovmentCounter[i] = 0;
        BODstate[i] = BOD::BODwalk;
    }
}

// update monster
void MonstersMovment() {
    for (int i = 0; i < BODnumber; i++) {

        // hide BOD spell
        showBODSpell[i] = false;

        // check that BOD is alive
        if (!BODalive[i])
            continue;
        if (BODstate[i] == BOD::BODdie) {
            die(i);
            continue;
        }

        room_cleared = false;
        if (BODstate[i] == BOD::BODspawn) {
            spawn(i);
            continue;
        }

        // check distance between BOD and Player and make BOD look torward Player
        double x = Player.getPosition().x - BODmonsters[i].BOD.getPosition().x, y = BODmonsters[i].BOD.getPosition().y - Player.getPosition().y;
        if (x > 0)
            BODmonsters[i].BOD.setScale(Vector2f(-2.3, 2.3));
        else
            BODmonsters[i].BOD.setScale(Vector2f(2.3, 2.3));


        // passing time for cooldown
        BODmonsters[i].cooldown -= playerdeltatime;

        // check if BOD is being attacked
        if (BODstate[i] != BOD::BODhurt && abs(x) < 100 && abs(y) < 100) {
            if (curr_state == player_base) {
                BODmonsters[i].health -= round((damageUp * attackUpCof +1) * 1); // damage up is the attack upgrade level
                MovmentCounter[i] = 0;
                BODstate[i] = BOD::BODhurt;
            }
            else if (curr_state == player_xmove) {
                BODmonsters[i].health -= round((damageUp * attackUpCof + 1) * 3);
                MovmentCounter[i] = 0;
                BODstate[i] = BOD::BODhurt;
            }
            else if (curr_state == player_cmove) {
                BODmonsters[i].health -= round((damageUp * attackUpCof + 1) * 5);
                MovmentCounter[i] = 0;
                BODstate[i] = BOD::BODhurt;
            }
            else if (curr_state == player_vmove) {
                BODmonsters[i].health -= round((damageUp * attackUpCof + 1) * 10);
                MovmentCounter[i] = 0;
                BODstate[i] = BOD::BODhurt;
            }
        }

        // check if BOD is doing somthing
        if (BODstate[i] == BOD::BODhurt) {
            if (BODmonsters[i].health <= 0) {
                die(i);
                BODstate[i] = BOD::BODdie;
            }
            else
                hurt(i);
            continue;
        }
        else if (BODstate[i] == BOD::BODcast) {
            cast(i);
            showBODSpell[i] = true;
            continue;
        }
        else if (BODstate[i] == BOD::BODattack) {
            attack(x, y, i);
            continue;
        }



        // making BOD decision
        if ((long long)abs(x) * abs(x) + abs(y) * abs(y) < 100000 && BODmonsters[i].cooldown <= 0) {
            BODmonsters[i].spell.setPosition(Player.getPosition().x - 100, Player.getPosition().y - 200);
            MovmentCounter[i] = 0;
            BODstate[i] = BOD::BODcast;
            cast(i);
            showBODSpell[i] = true;
            BODmonsters[i].cooldown = 5;
        }
        else if (abs(x) < 300 && abs(y) < 30) {
            MovmentCounter[i] = 0;
            BODstate[i] = BOD::BODattack;
            attack(x, y, i);
        }
        else
            Bringerwalk(x, y, i);
    }
}

// set monsters at the begining of the wave
void SetMonsters() {
    for (int i = 0; i < BODnumber; i++) {
        BODalive[i] = true;
        MonsterCounter[i] = 0;
        MovmentCounter[i] = 0;
        BODstate[i] = BOD::BODspawn;
        BODmonsters[i] = BODoriginal;
        int x, y;
        RandPosition(x, y);
        BODmonsters[i].BOD.setPosition(x,y);
    }
}

// create monsters when opening the game
void CreateMonsters() {
    BODtexture.loadFromFile("enemies/Bringer-Of-Death/SpriteSheet/Bringer-of-Death-SpritSheet.png");
    BODoriginal.health = 10;
    BODoriginal.spell.setTexture(BODtexture);
    BODoriginal.spell.setTextureRect(getRect(48));
    BODoriginal.spell.setScale(1.7, 1.7);
    BODoriginal.BOD.setTexture(BODtexture);
    BODoriginal.BOD.setTextureRect(getRect(0));
    BODoriginal.BOD.setOrigin(105, 62);
    BODoriginal.BOD.setScale(1.7, 1.7);
}