#include "includes.h"
#include "Monsters.h"
#include "globals.h"
#include "Skeleton.h"
#include "ShardSoul.h"
#include "BrainMole.h"
#include "Rogue.h"
#include "cthulu.h"
#include "Demon.h"
#include "RandomizePlaces.h"

void SetMonstersSprites() {
	CreateMonsters();
    AACreate();
    NBcreate();
    create_goblins();
    Screate();
    SScreate();
    BMcreate();
    Rcreate();
    Ccreate();
    FGcreate();
    Dcreate();
    FWcreate();

}

void SetMonstersWave() {
    MonsterNumber();
	SetMonsters();
    NBset(NightBroneNumber);
    AASet(ArcaneArcherNumber);
    set_goblins(GoblinNumber);
    Sset(SkeletonNumber);
    SSset(ShardSoulNumber);
    BMset(BrainMoleNumber);
    Rset(RogueNumber);
    Cset(CthuluNumber);
    FGset(FrostGuardianNumber);
    Dset(DemonNumber);
    FWset(FlameWizardNumber);
}

void MoveMonsters() {
    int attct = 0;
    if (curr_state == player_base)
        attct = round((damageUp * attackUpCof + 1) * 1); // damageUp is the attack upgrade level 
    else if (curr_state == player_xmove)
        attct = round((damageUp * attackUpCof + 1) * 3);
    else if (curr_state == player_cmove)
        attct = round((damageUp * attackUpCof + 1) * 5);
    else if (curr_state == player_vmove)
        attct = round((damageUp * attackUpCof + 1) * 10);
    room_cleared = true;
	MonstersMovment();
    Rmove(playerdeltatime, Player, attct, Player_Health);
    NBmove(playerdeltatime, Player, attct, Player_Health, ishit);
    AAMove(playerdeltatime,Player,attct,Player_Health,ishit);
    goblins_move(GoblinNumber, attct);
    Smove(playerdeltatime, Player, attct, Player_Health, ishit);
    SSmove(playerdeltatime, Player, attct, Player_Health);
    BMmove(playerdeltatime, Player, attct, Player_Health);
    Ctmove(playerdeltatime, Player, attct, Player_Health);
    FGmove(playerdeltatime, Player, attct, Player_Health);
    Dmove(playerdeltatime, Player, attct, Player_Health);
    FWmove(playerdeltatime, Player, attct, Player_Health);
    room_cleared = true;
}

void ShowMonsters() {
    for (int i = 0; i < BODnumber; i++)
        if (BODalive[i]) {
            window.draw(BODmonsters[i].BOD);
            gui.drawBODHP(BODmonsters[i].BOD.getPosition(), BODmonsters[i].health / 10.0);
            if (showBODSpell[i])
                window.draw(BODmonsters[i].spell);
        }
    draw_goblin(window, GoblinNumber);
    Sdraw(window);
    SSdraw(window);
    BMdraw(window);
    AADraw(window);
    NBdraw(window);
    Rdraw(window);
    Cdraw(window);
    FGdraw(window);
    Ddraw(window);
    FWdraw(window);
}

void ResetMonsters() {
    BODnumber = 0;
    ArcaneArcherNumber = 0;
    NightBroneNumber = 0;
    SkeletonNumber = 0;
    ShardSoulNumber = 0;
    BrainMoleNumber = 0;
    RogueNumber = 0;
    GoblinNumber = 0;
    CthuluNumber = 0;
    FrostGuardianNumber = 0;
    DemonNumber = 0;
    FlameWizardNumber = 0;
}