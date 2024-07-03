#include "includes.h"
#include "Monsters.h"
#include "globals.h"
#include "GUI.h"
#include "BrainMole.h"

// Define global variables here
int globalInt = 0;  // Definition
Clock GameClock;
View view(Vector2f(0, 0), Vector2f(1920, 1080));

int level = 1;
int AblazeCharge = 100;
bool AblazeReady = false;
bool Ablaze = false;
bool room_cleared = true;
int current_room = 0,current_wave = 0;
int ArcaneArcherNumber, NightBroneNumber, SkeletonNumber, ShardSoulNumber, BrainMoleNumber, RogueNumber, GoblinNumber, CthuluNumber, FrostGuardianNumber, DemonNumber;
RenderWindow window(VideoMode(1920, 1080), "Dungeon Slayer", Style::Fullscreen);
int BODnumber;
BringerOfDeath BODmonsters[30];
NightBrone NBmonsters[30];
ShardSoul SSmonsters[30];
Skeleton Smonsters[100];
BringerOfDeath BODoriginal;
BrainMole BMmonsters[30];
Cthulu Cmonsters[5];
FrostGuardian FGmonsters[5];
Demon Dmonsters[5];
Texture BODtexture;
bool showBODSpell[30], BODalive[30];
Sprite Player;
float playerdeltatime = 0;
int Player_Health = 100;
int Max_Player_Health = 100;
float damageUp = 0;
int damageUpCost = 20;
int cooldownUpCost = 20;
int healthUpCost = 20;
float attackUpCof = 0.1;
float AblazeDuration = 25;
float healthUp = 0;
float cooldownUp = 0;
float coinsCount=40;
bool ishit = false;
float cooldown[5];
GUI gui;
enum state
{
    idle, run, hit, base,  xmove, cmove, vmove, dead, walk
};
state curr_state = state::idle, player_base = state::base, player_vmove = state::vmove, player_xmove = state::xmove, player_cmove = state::cmove;
state player_idle = state::idle, player_run = state::run, player_hit = state::hit, player_dead = state::dead, player_walk = state::walk;

BOD BODstate[30], BODattacks = BOD::BODattack;

//
RectangleShape borderR1(Vector2f({ 100,1000 }));
RectangleShape borderR2(Vector2f({ 50,200 }));
RectangleShape borderR3(Vector2f({ 50,800 }));
RectangleShape borderR4(Vector2f({ 50,100 }));
RectangleShape borderR5(Vector2f({ 50,1000 }));
RectangleShape borderR6(Vector2f({ 50,200 }));
RectangleShape borderR7(Vector2f({ 50,150 }));
RectangleShape borderR8(Vector2f({ 50,150 }));
RectangleShape borderR9(Vector2f({ 50,1000 }));
RectangleShape borderR10(Vector2f({ 100,50 }));
RectangleShape borderR11(Vector2f({ 50,250 }));
RectangleShape borderR12(Vector2f({ 150,50 }));
RectangleShape borderR13(Vector2f({ 50,900 }));
RectangleShape borderR14(Vector2f({ 75,50 }));
RectangleShape borderR15(Vector2f({ 50,200 }));
RectangleShape borderR16(Vector2f({ 50,500 }));
RectangleShape borderR17(Vector2f({ 50,600 }));
RectangleShape borderR18(Vector2f({ 50,600 }));
RectangleShape borderR19(Vector2f({ 50,800 }));
RectangleShape borderR20(Vector2f({ 50,775 }));
RectangleShape borderR21(Vector2f({ 10,10 }));
RectangleShape borderR22(Vector2f({ 10,10 }));
RectangleShape borderR23(Vector2f({ 10,10 }));
RectangleShape borderR24(Vector2f({ 10,10 }));
RectangleShape borderR25(Vector2f({ 10,10 }));

RectangleShape borderL1(Vector2f({ 50,1000 }));
RectangleShape borderL2(Vector2f({ 50,200 }));
RectangleShape borderL3(Vector2f({ 50,800 }));
RectangleShape borderL4(Vector2f({ 50, 100 }));
RectangleShape borderL5(Vector2f({ 50, 1000 }));
RectangleShape borderL6(Vector2f({ 50, 200 }));
RectangleShape borderL7(Vector2f({ 50, 150 }));
RectangleShape borderL8(Vector2f({ 50, 150 }));
RectangleShape borderL9(Vector2f({ 50, 1000 }));
RectangleShape borderL10(Vector2f({ 100, 50 }));
RectangleShape borderL11(Vector2f({ 50, 250 }));
RectangleShape borderL12(Vector2f({ 150, 50 }));
RectangleShape borderL13(Vector2f({ 50, 900 }));
RectangleShape borderL14(Vector2f({ 75,50 }));
RectangleShape borderL15(Vector2f({ 50,200 }));
RectangleShape borderL16(Vector2f({ 50,500 }));
RectangleShape borderL17(Vector2f({ 50,600 }));
RectangleShape borderL18(Vector2f({ 50,600 }));
RectangleShape borderL19(Vector2f({ 50, 775 }));
RectangleShape borderL20(Vector2f({ 50,800 }));
RectangleShape borderL21(Vector2f({ 10,10 }));
RectangleShape borderL22(Vector2f({ 10,10 }));
RectangleShape borderL23(Vector2f({ 10,10 }));
RectangleShape borderL24(Vector2f({ 10,10 }));
RectangleShape borderL25(Vector2f({ 10,10 }));

RectangleShape borderU1(Vector2f({ 700,50 }));
RectangleShape borderU2(Vector2f({ 700,50 }));
RectangleShape borderU3(Vector2f({ 100,50 }));
RectangleShape borderU4(Vector2f({ 100,50 }));
RectangleShape borderU5(Vector2f({ 900,50 }));
RectangleShape borderU6(Vector2f({ 900,50 }));
RectangleShape borderU7(Vector2f({ 100,50 }));
RectangleShape borderU8(Vector2f({ 100,50 }));
RectangleShape borderU9(Vector2f({ 600,50 }));
RectangleShape borderU10(Vector2f({ 600,50 }));
RectangleShape borderU11(Vector2f({ 75,50 }));
RectangleShape borderU12(Vector2f({ 75,50 }));
RectangleShape borderU13(Vector2f({ 1000,50 }));
RectangleShape borderU14(Vector2f({ 1000,50 }));
RectangleShape borderU15(Vector2f({ 250,50 }));
RectangleShape borderU16(Vector2f({ 600,50 }));
RectangleShape borderU17(Vector2f({ 600,50 }));
RectangleShape borderU18(Vector2f({ 250,50 }));
RectangleShape borderU19(Vector2f({ 2000,50 }));
RectangleShape borderU20(Vector2f({ 10,10 }));
RectangleShape borderU21(Vector2f({ 10,10 }));
RectangleShape borderU22(Vector2f({ 10,10 }));
RectangleShape borderU23(Vector2f({ 10,10 }));
RectangleShape borderU24(Vector2f({ 10,10 }));

RectangleShape borderD1(Vector2f({ 2000,50 }));
RectangleShape borderD2(Vector2f({ 700,50 }));
RectangleShape borderD3(Vector2f({ 700,50 }));
RectangleShape borderD4(Vector2f({ 100,50 }));
RectangleShape borderD5(Vector2f({ 100,50 }));
RectangleShape borderD6(Vector2f({ 600,50 }));
RectangleShape borderD7(Vector2f({ 600,50 }));
RectangleShape borderD8(Vector2f({ 50,50 }));
RectangleShape borderD9(Vector2f({ 50,50 }));
RectangleShape borderD10(Vector2f({ 50,50 }));
RectangleShape borderD11(Vector2f({ 50,50 }));
RectangleShape borderD12(Vector2f({ 1000,50 }));
RectangleShape borderD13(Vector2f({ 1000,50 }));
RectangleShape borderD14(Vector2f({ 150,50 }));
RectangleShape borderD15(Vector2f({ 150,50 }));
RectangleShape borderD16(Vector2f({ 750,50 }));
RectangleShape borderD17(Vector2f({ 750,50 }));
RectangleShape borderD18(Vector2f({ 250,50 }));
RectangleShape borderD19(Vector2f({ 250,50 }));
RectangleShape borderD20(Vector2f({ 550,50 }));
RectangleShape borderD21(Vector2f({ 550,50 }));
RectangleShape borderD22(Vector2f({ 10,10 }));
RectangleShape borderD23(Vector2f({ 10,10 }));
RectangleShape borderD24(Vector2f({ 10,10 }));
RectangleShape borderD25(Vector2f({ 10,10 }));
RectangleShape borderD26(Vector2f({ 10,10 }));


RectangleShape right_borders[] = { borderR1 , borderR2 , borderR3 , borderR4 ,borderR5, borderR6, borderR7, borderR8, borderR9, borderR10 ,
    borderR11, borderR12 , borderR13 , borderR14 , borderR15 , borderR16 , borderR17, borderR18, borderR19 ,borderR20, borderR21 , borderR22,
    borderR23, borderR24 , borderR25
};

RectangleShape left_borders[] = { borderL1,borderL2 , borderL3 , borderL4, borderL5 , borderL6 , borderL7, borderL8, borderL9, borderL10,
    borderL11 , borderL12 , borderL13 , borderL14 , borderL15 , borderL16 ,borderL17, borderL18, borderL19 , borderL20, borderL21, borderL22,
    borderL23 ,borderL24 , borderL25
};

RectangleShape up_borders[] = { borderU1, borderU2 , borderU3 , borderU4, borderU5, borderU6, borderU7, borderU8, borderU9, borderU10,
    borderU11,borderU12 ,borderU13, borderU14 , borderU15 , borderU16, borderU17, borderU18, borderU19,borderU20 , borderU21, borderU22,
    borderU23 , borderU24
};

RectangleShape down_borders[] = { borderD1,borderD2, borderD3 , borderD4, borderD5, borderD6, borderD7, borderD8, borderD9 ,borderD10,
    borderD11 , borderD12 , borderD13 , borderD14 , borderD15 , borderD16, borderD17, borderD18 , borderD19 ,borderD20, borderD21 ,
    borderD22 , borderD23, borderD24 , borderD25 , borderD26
};