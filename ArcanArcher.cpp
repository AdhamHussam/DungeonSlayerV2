#include "ArcanArcher.h"
#include "RandomizePlaces.h"
#include "globals.h"

Texture AAtexture,AAArow;
ArcanArcher AAoriginal,AAmonsters[30];
int AAMovmentCounter[30];
float AAMonsterCounter[30],AAdeltatime;
Sprite player;

enum AAenum {
    AA_walk, AA_attack, AA_hurt, AA_die, AA_spawn
};
AAenum AAstate[30];

IntRect AAgetRect(int pos) {
    int x = pos % 8;
    int y = pos / 8;
    return IntRect(x * 64, y * 64, 64, 64);
}

void AAUpdateMonsterAnimationCounter(int i, float st = 0.15) {
    AAMonsterCounter[i] += AAdeltatime;
    if (AAMonsterCounter[i] >= st)
    {
        AAMonsterCounter[i] = 0;
        AAMovmentCounter[i]++;
    }
}

void AAwalk(int i) {
    AAmonsters[i].AA.setTextureRect(AAgetRect(AAMovmentCounter[i]));
    Vector2f Direction = player.getPosition() - AAmonsters[i].AA.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    AAmonsters[i].AA.move(Vector2f(norm_direction.x * AAmonsters[i].speed * AAdeltatime, norm_direction.y * AAmonsters[i].speed * AAdeltatime));
    CheckMonsterCollisions(AAmonsters[i].AA, norm_direction.x * AAmonsters[i].speed * AAdeltatime, norm_direction.y * AAmonsters[i].speed * AAdeltatime);
    AAUpdateMonsterAnimationCounter(i);
    AAMovmentCounter[i] %= 8;
}

float AAcalculateAngle(const sf::Vector2f& from, const sf::Vector2f& to) {
    return atan2(to.y - from.y, to.x - from.x) * 180 / 3.14159;
}

void AAattack(int i) {
    AAmonsters[i].AA.setTextureRect(AAgetRect(24 + AAMovmentCounter[i]));
    AAUpdateMonsterAnimationCounter(i);
    if (AAMovmentCounter[i] == 7){
        AAMovmentCounter[i] = 0;
        AAmonsters[i].target = player.getPosition();
        AAmonsters[i].arrow.setPosition(AAmonsters[i].AA.getPosition());
        AAmonsters[i].arrow.setRotation(AAcalculateAngle(AAmonsters[i].arrow.getPosition(),AAmonsters[i].target));
        AAmonsters[i].hitted = false;
        AAstate[i] = AAenum::AA_walk;
    }
}

void AAdie(int i) {
    AAmonsters[i].AA.setTextureRect(AAgetRect(8 + AAMovmentCounter[i]));
    AAUpdateMonsterAnimationCounter(i);
    if (AAMovmentCounter[i] == 8) {
        AAmonsters[i].alive = false;
        AblazeCharge+=2;
    }
}

void AAspawn(int i) {
    AAmonsters[i].AA.setTextureRect(AAgetRect(48 + AAMovmentCounter[i]));
    AAUpdateMonsterAnimationCounter(i,0.3);
    if (AAMovmentCounter[i] == 4) {
        AAMovmentCounter[i] = 0;
        AAmonsters[i].power *= 2;
        AAmonsters[i].health++;
        AAmonsters[i].max_health++;
        AAmonsters[i].ArrowSpeed += 100;
        AAstate[i] = AAenum::AA_walk;
        AAmonsters[i].cooldown = 10;
    }
}

void AAhurt(int i) {
    AAmonsters[i].AA.setTextureRect(AAgetRect(40 + AAMovmentCounter[i]));
    AAUpdateMonsterAnimationCounter(i);
    if (AAMovmentCounter[i] == 4) {
        AAMovmentCounter[i] = 0;
        AAstate[i] = AAenum::AA_walk;
    }
}

void AAArowFire(int i) {
    Vector2f Direction = AAmonsters[i].target - AAmonsters[i].arrow.getPosition();
    float magnitude = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
    Vector2f norm_direction = Direction / magnitude;
    AAmonsters[i].arrow.move(Vector2f(norm_direction.x * AAmonsters[i].ArrowSpeed * AAdeltatime, norm_direction.y * AAmonsters[i].ArrowSpeed * AAdeltatime));
}

void AACreate() {
    AAArow.loadFromFile("enemies/Arcane archer/projectile.png");
    AAoriginal.health = AAoriginal.max_health = 5;
    AAoriginal.power = 4;
    AAoriginal.cooldown = 10;
    AAoriginal.arrow.setTexture(AAArow);
    AAoriginal.arrow.setScale(2.5, 2.5);
    AAoriginal.arrow.setPosition(100000, 100000);
    AAoriginal.ArrowSpeed = 2000;
    AAoriginal.target = { 100000,100000 };
    AAtexture.loadFromFile("enemies/Arcane archer/spritesheet.png");
    AAoriginal.AA.setTexture(AAtexture);
    AAoriginal.AA.setTextureRect(AAgetRect(0));
    AAoriginal.AA.setOrigin(32, 32);
    AAoriginal.AA.setScale(2.5, 2.5);
    AAoriginal.alive = true;
}

void AASet(int AAn) {
    for (int i = 0; i < ArcaneArcherNumber; i++) {
        AAmonsters[i] = AAoriginal;
        AAMonsterCounter[i] = 0;
        AAMovmentCounter[i] = 0;
        int x, y;
        RandPosition(x, y);
        AAmonsters[i].AA.setPosition(x,y);
        AAmonsters[i].alive = true;
        AAstate[i] = AAenum::AA_spawn;
        if (level == 2) {
            AAmonsters[i].power = 15;
            AAmonsters[i].health = AAmonsters[i].max_health = 250;
        }
    }
}

void AAMove(float time,Sprite p,int attct, int& PlayerHealth, bool& IsHit) {
    player = p;
    AAdeltatime = time;
    for (int i = 0; i < ArcaneArcherNumber; i++) {

        // check if alive
        if (!AAmonsters[i].alive)
            continue;

        room_cleared = false;
        if (abs(AAmonsters[i].arrow.getPosition().x - AAmonsters[i].target.x) > 2) {
            AAArowFire(i);
        }
        if (!AAmonsters[i].hitted && abs(AAmonsters[i].arrow.getPosition().x - player.getPosition().x) < 20 && abs(AAmonsters[i].arrow.getPosition().y - player.getPosition().y) < 20) {
            PlayerHealth -= AAmonsters[i].power;
            AAmonsters[i].hitted = IsHit = true;
        }

        // check if AA will die
        if (AAmonsters[i].health <= 0 && AAstate[i] != AAenum::AA_die) {
            AAMovmentCounter[i] = 0;
            AAstate[i] = AAenum::AA_die;
        }

        // check if AA is dying
        if (AAstate[i] == AAenum::AA_die) {
            AAdie(i);
            continue;
        }

        // check if AA is spawning
        if (AAstate[i] == AAenum::AA_spawn) {
            AAspawn(i);
            continue;
        }

        double x = player.getPosition().x - AAmonsters[i].AA.getPosition().x, y = AAmonsters[i].AA.getPosition().y - player.getPosition().y;
        if(level != 2){
            if (x < 0)
                AAmonsters[i].AA.setScale(Vector2f(-2.5, 2.5));
            else
                AAmonsters[i].AA.setScale(Vector2f(2.5, 2.5));
        }
        else {
            if (x < 0)
                AAmonsters[i].AA.setScale(Vector2f(-5, 5));
            else
                AAmonsters[i].AA.setScale(Vector2f(5, 5));
        }
        AAmonsters[i].cooldown -= AAdeltatime;

        // check if BOD is being attacked
        if (AAstate[i] != AAenum::AA_hurt && abs(x) < 100 && abs(y) < 100 && attct) {
            AAmonsters[i].health-=attct;
            AAMovmentCounter[i] = 0;
            AAstate[i] = AAenum::AA_hurt;
        }

        // check if doing somthing
        if(AAstate[i] == AAenum::AA_hurt) {
            AAhurt(i);
            continue;
        }
        else if (AAstate[i] == AAenum::AA_attack) {
            AAattack(i);
            continue;
        }

        // make decision
        if (AAmonsters[i].cooldown <= 0) {
            AAspawn(i);
            AAstate[i] = AAenum::AA_spawn;
        }
        else if (abs(x) * abs(x) + abs(y) * abs(y) < 600000){
            AAstate[i] = AAenum::AA_attack;
            AAattack(i);
        }
        else
            AAwalk(i);
    }
}

void AADraw(RenderWindow &window) {
    for (int i = 0; i < ArcaneArcherNumber; i++) {
        if(AAmonsters[i].alive) {
            window.draw(AAmonsters[i].AA);
            window.draw(AAmonsters[i].arrow);
            if (level == 2)
                gui.drawBossHP(AAmonsters[i].health, AAmonsters[i].max_health);
            else
                gui.DrawMonsterHP(AAmonsters[i].AA.getPosition(),
                    AAmonsters[i].health, AAmonsters[i].max_health, 25, 60);
        }
    }
}