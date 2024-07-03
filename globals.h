#include "GUI.h"
#include "BrainMole.h"
#include "NightBorne.h"
#include "ShardSoul.h"
#include "Skeleton.h"
#include "cthulu.h"
#include "FrostGuardian.h"
#include "Demon.h"
#include "FlameWizard.h"
#include "Cacodaemon.h"
#include "FireKnight.h"
#include "Ghoul.h"
#include "IceViking.h"
#include "CrystalMauler.h"

#ifndef GLOBALS_H
#define GLOBALS_H

// Declare global variables here
extern int globalInt;  // Declaration
extern Clock GameClock;
extern View view;

extern int level;
extern int AblazeCharge;
extern bool room_cleared;
extern int ArcaneArcherNumber, NightBroneNumber, SkeletonNumber, ShardSoulNumber, BrainMoleNumber, RogueNumber, GoblinNumber, CthuluNumber, FrostGuardianNumber, DemonNumber, FlameWizardNumber, CacodaemonNumber, FireKnightNumber,GhoulNumber;
extern bool Ablaze;
extern bool AblazeReady;
extern bool is1;
extern RenderWindow window;
/*

*/
extern int current_room,current_wave,BODnumber, IceVikingNumber, CrystalMaulerNumber;
extern Sprite Player;
extern float playerdeltatime;
enum state;
extern state curr_state, player_idle, player_run, player_hit, player_base,  player_xmove, player_cmove, player_vmove, player_dead, player_walk;
enum BOD;
extern BOD BODstate[], BODattacks;
extern int Player_Health;
extern int Max_Player_Health;
extern bool ishit;
extern float cooldown[5];
extern float damageUp;
extern float AblazeDuration;
extern int damageUpCost;
extern float attackUpCof;
extern float healthUp;	
extern int healthUpCost;
extern int fireUltFrame;
extern int cooldownUpCost;
extern float cooldownUp;
extern float coinsCount;
extern BrainMole BMmonsters[30];
extern NightBrone NBmonsters[30];
extern ShardSoul SSmonsters[30];
extern Skeleton Smonsters[100];
extern Cthulu Cmonsters[5];
extern FrostGuardian FGmonsters[5];
extern Demon Dmonsters[5];
extern FlameWizard FWmonsters[30];
extern Cacodaemon CDmonsters[30];
extern FireKnight FKmonsters[30];
extern Ghoul Gmonsters[30];
extern IceViking IVmonsters[30];
extern CrystalMauler CMmonsters[30];
extern GUI gui;

extern RectangleShape borderR1;
extern RectangleShape borderR2;
extern RectangleShape borderR3;
extern RectangleShape borderR4;
extern RectangleShape borderR5;
extern RectangleShape borderR6;
extern RectangleShape borderR7;
extern RectangleShape borderR8;
extern RectangleShape borderR9;
extern RectangleShape borderR10;
extern RectangleShape borderR11;
extern RectangleShape borderR12;
extern RectangleShape borderR13;
extern RectangleShape borderR14;
extern RectangleShape borderR15;
extern RectangleShape borderR16;
extern RectangleShape borderR17;
extern RectangleShape borderR18;
extern RectangleShape borderR19;
extern RectangleShape borderR20;


extern RectangleShape borderL1;
extern RectangleShape borderL2;
extern RectangleShape borderL3;
extern RectangleShape borderL4;
extern RectangleShape borderL5;
extern RectangleShape borderL6;
extern RectangleShape borderL7;
extern RectangleShape borderL8;
extern RectangleShape borderL9;
extern RectangleShape borderL10;
extern RectangleShape borderL11;
extern RectangleShape borderL12;
extern RectangleShape borderL13;
extern RectangleShape borderL14;
extern RectangleShape borderL15;
extern RectangleShape borderL16;
extern RectangleShape borderL17;
extern RectangleShape borderL18;
extern RectangleShape borderL19;
extern RectangleShape borderL20;


extern RectangleShape borderU1;
extern RectangleShape borderU2;
extern RectangleShape borderU3;
extern RectangleShape borderU4;
extern RectangleShape borderU5;
extern RectangleShape borderU6;
extern RectangleShape borderU7;
extern RectangleShape borderU8;
extern RectangleShape borderU9;
extern RectangleShape borderU10;
extern RectangleShape borderU11;
extern RectangleShape borderU12;
extern RectangleShape borderU13;
extern RectangleShape borderU14;
extern RectangleShape borderU15;
extern RectangleShape borderU16;
extern RectangleShape borderU17;
extern RectangleShape borderU18;
extern RectangleShape borderU19;


extern RectangleShape borderD1;
extern RectangleShape borderD2;
extern RectangleShape borderD3;
extern RectangleShape borderD4;
extern RectangleShape borderD5;
extern RectangleShape borderD6;
extern RectangleShape borderD7;
extern RectangleShape borderD8;
extern RectangleShape borderD9;
extern RectangleShape borderD10;
extern RectangleShape borderD11;
extern RectangleShape borderD12;
extern RectangleShape borderD13;
extern RectangleShape borderD14;
extern RectangleShape borderD15;
extern RectangleShape borderD16;
extern RectangleShape borderD17;
extern RectangleShape borderD18;
extern RectangleShape borderD19;
extern RectangleShape borderD20;
extern RectangleShape borderD21;

extern RectangleShape right_borders[];

extern RectangleShape left_borders[];

extern RectangleShape up_borders[];

extern RectangleShape down_borders[];


#endif // GLOBALS_H
