#include "includes.h"
#include "Menu.h"
#include "GameOver.h"
#include "Monsters.h"
#include "globals.h"
#include "PauseMenu.h"
#include "GUI.h"

// Game properties
enum state
{
    idle, run, hit, base, xmove, cmove, vmove, dead, walk
}; 

enum NPCstate
{
    npc_idle, npc_talk
};

NPCstate upgrade_npc_state = NPCstate::npc_idle;
NPCstate trade_npc_state = NPCstate::npc_idle;

int doors = 5;
int right_walls = 25;
int left_walls = 25;
int up_walls = 24;
int down_walls = 26;

// music triggers

bool openertrigger = false;
bool map_opener_trigger = false;
bool death_trigger = false;

// menu number
int pagenum = 10;

Text go_next_text;
Text set_your_heart_text;
Text upgrader_text;
Text Skip_text;
Text CutScenetext;
Font game_font;

// player attributes
float lastX = 0 , lastY = 0;
float player_scale = 0.2;
float animation_multiplier = 1;
float button_lag = 0.2;
int walk_speed = 100;
int run_speed = 200;
int cooldown_divider = 1;
Vector2f velocity = { 0, 0 };
float AnimationCounter = 0;
float upgradeNPCcounter = 0;
float tradeNPCcounter = 0;
float PowerUpcounter = 0;
float dash_duration = 0.115;
int maximagecounter = 0;
int ImageCounter = 0;
int upgradeNPCImageCounter = 0;
int PowerUpImageCounter = 0;
int tradeNPCImageCounter = 0;

float intensity = 35.0;

bool animation_running = false;
bool isDead = false;
bool go_next = false;
bool ispassing = false;
bool passed_door = false;
bool isdashing = false;
bool finishedanimationonce = false;
bool power_up = false;
bool shopOpened = false;
bool shopNear = false;
bool cutScenePlaying = false;


Menu menu(1920, 1080);
PauseMenu pause(1920, 1080);

Clock cutscene;
Clock pausetimer;
Clock attacktimer;
Clock dashtimer;
Clock upgradetimer;

Vector2f initial_position(-500, 7000);

// Textures

Texture Idle;
Texture Idle2;
Texture DeathAnimation[3];
Texture HitAnimation[3];
Texture BaseAttack[8];
Texture BaseAttackUlt[8];
Texture RunAnimation[8];
Texture Vmove[7];
Texture Xmove[7];
Texture Cmove[8];
Texture RunAnimationUlt[8];
Texture VmoveUlt[7];
Texture XmoveUlt[7];
Texture CmoveUlt[8];
Texture walkAnimation[8];
Texture powerup[5];

Texture map1;
Texture map2;
Texture map3;
Texture map4;
Texture map5;

Texture upgrade_npc_idle[4];
Texture upgrade_npc_talk[4];

Texture trade_npc_idle[11];
Texture trade_npc_talk[11];

Texture Pscene1;
Texture Mscene1[4];

Sprite Pscene1s;
Sprite Mscene1s;

Sprite UpgradeNPC;
Sprite TradeNPC;

CircleShape shadow1;
CircleShape shadow2;

Sprite Powerup;
Texture mainmenubg;
Texture credits;
Texture death_screen;
Sprite DeathScreen;
Sprite Credits;
Sprite bg;
Sprite Room;
Texture pausebg;
Sprite pausemenu;
Sprite Map;
IntRect monsterScene1SA(0, 0, 45, 40);
Texture playerScene1;
Texture monsterScene1;
Texture textbox;
Sprite textboxSP;
Sprite textboxSM; 
Sprite playerScene1S; 
Sprite monsterScene1S(monsterScene1, monsterScene1SA); 
// Room 0 Borders
RectangleShape gate1(Vector2f({ 1, 1 }));
RectangleShape gate2(Vector2f({ 1, 1 }));
RectangleShape gate3(Vector2f({ 1, 1 }));
RectangleShape gate4(Vector2f({ 1, 1 }));
RectangleShape gate5(Vector2f({ 1, 1 }));

RectangleShape finalgate(Vector2f({ 100, 250 }));
RectangleShape gates[] = { gate1, gate2 , gate3, gate4, gate5 , finalgate};

SoundBuffer menu_opener;
Sound MenuOpener;

SoundBuffer death_sound;
Sound DeathSound;

SoundBuffer game_music;
Sound GameMusic;

SoundBuffer player_attack1;
SoundBuffer player_attackX;
SoundBuffer player_attackC;
SoundBuffer player_attackV;
SoundBuffer player_hurt;
SoundBuffer menu_sound;

Sound PlayerAttack;
Sound PlayerHurt;
Sound MenuSounds;


// Game functions

void menu_handler();
void music_handler();
void Switch_States();
void Game_play(RenderWindow& window);
void PauseMenuHandler(RenderWindow& window);
void Credits_Menu(RenderWindow& window);
void Credits_Draw();
void update();
void dash();
void checkpause();
void check_room();
void death_handler();
void trackView();
void playerMovement();
void setNormalMovesTexture();
void setAblazeMovesTexture();
void setTextures();
void checkCollisions();
void Draw();
void upgradeShop();
void UpdateAnimationCounter(float st = 0.1);
void upgradeNpcAnimation(float st);
void tradeNpcAnimation(float st);
void powerupAnimation(float st);
void game_reset();
void Go_Next();
void check_ablaze();
void set_your_heart_ablaze();
void camera_shake();
void cutScene();

// Main 
int main()
{
    window.setMouseCursorVisible(false);
    SetMonstersSprites();
    setTextures();
    menu_handler();
    return 0;
}

// Definitions;
void update() {
    if (!isDead) {
        Switch_States();
        playerMovement();
        MoveMonsters();
        check_room();
        trackView();
        checkCollisions();
        checkpause();
        dash();
        check_ablaze();
        set_your_heart_ablaze();
        camera_shake();
        window.setView(view);
    }
    else death_handler();
 
}

void checkCollisions() {

    //upgrader
    upgradeShop();
    //doors
    for (int i = 0; i < doors; i++) {
        if (Player.getGlobalBounds().intersects(gates[i].getGlobalBounds())) {
            
            if (room_cleared)
            {
                ispassing = true;
                Player.move(0, -500 * playerdeltatime);
            }
            else {
                if (Keyboard::isKeyPressed(Keyboard::S) && i == current_room - 1)
                    velocity.y = 0; 
                if (Keyboard::isKeyPressed(Keyboard::W) && i == current_room)
                    velocity.y = 0;
            }
          
            break;
        }
        else ispassing = false;
    }

    if (room_cleared && Player.getGlobalBounds().intersects(gates[5].getGlobalBounds())) {
        Go_Next();
    }
    else go_next = false;
    
    // right
    for (int i = 0; i < right_walls; i++) {
        if (Keyboard::isKeyPressed(Keyboard::D) && Player.getGlobalBounds().intersects(right_borders[i].getGlobalBounds())) {
            velocity.x = 0;
        }
    }

    //left
    for (int i = 0; i < left_walls; i++) {
        if (Keyboard::isKeyPressed(Keyboard::A) && Player.getGlobalBounds().intersects(left_borders[i].getGlobalBounds())) {
            velocity.x = 0;
        }
    }

    // up
    for (int i = 0; i < up_walls; i++) {
        if (Keyboard::isKeyPressed(Keyboard::W) && Player.getGlobalBounds().intersects(up_borders[i].getGlobalBounds())) {
            velocity.y = 0;
        }
    }

    //down
    for (int i = 0; i < down_walls; i++) {
        if (Keyboard::isKeyPressed(Keyboard::S) && Player.getGlobalBounds().intersects(down_borders[i].getGlobalBounds())) {
            velocity.y = 0;
        }
    }
  
}

void setNormalMovesTexture() {

    for (int i = 0; i < 8; i++) {
        BaseAttack[i].loadFromFile("base/Base" + to_string(i) + ".png");
    }
    for (int i = 0; i < 8; i++) {
        RunAnimation[i].loadFromFile("Run/run" + to_string(i) + ".png");
    }
    for (int i = 0; i < 7; i++) {
        Vmove[i].loadFromFile("V move/Vmove" + to_string(i) + ".png");
    }
    for (int i = 0; i < 7; i++) {
        Xmove[i].loadFromFile("X move/Xmove" + to_string(i) + ".png");
    }
    for (int i = 0; i < 8; i++) {
        Cmove[i].loadFromFile("C move/Cmove" + to_string(i) + ".png");
    }
}
void setAblazeMovesTexture() {
    
    for (int i = 0; i < 8; i++) {
        BaseAttackUlt[i].loadFromFile("base/BaseUlt" + to_string(i) + ".png");
    }
    for (int i = 0; i < 8; i++) {
        RunAnimationUlt[i].loadFromFile("Run/run" + to_string(i) + ".png");
    }
    for (int i = 0; i < 7; i++) {
        VmoveUlt[i].loadFromFile("V move/VmoveUlt" + to_string(i) + ".png");
    }
    for (int i = 0; i < 7; i++) {
        XmoveUlt[i].loadFromFile("X move/XmoveUlt" + to_string(i) + ".png");
    }
    for (int i = 0; i < 8; i++) {
        CmoveUlt[i].loadFromFile("C move/CmoveUlt" + to_string(i) + ".png");
    }
}
void setTextures() {

    // Menus   
    mainmenubg.loadFromFile("Main Menu.jpg");
    credits.loadFromFile("Credits.jpg");
    pausebg.loadFromFile("pausebg.png");
    death_screen.loadFromFile("death_screen.png");

    bg.setTexture(mainmenubg);
    Credits.setTexture(credits);
    pausemenu.setTexture(pausebg);
    DeathScreen.setTexture(death_screen);
    
    bg.setScale(0.5, 0.5);
    Credits.setScale(1, 1);
    pausemenu.setScale(0.5, 0.5);
    DeathScreen.setScale(0.7, 0.7);
    
    // Room

    map1.loadFromFile("lvl1.png");
    map2.loadFromFile("lvl2.png");
    map3.loadFromFile("lvl3.png");
    map4.loadFromFile("lvl4.png");
    map5.loadFromFile("lvl5.png");
    Map.setTexture(map1);
    Map.setScale(3.8, 3.333);
    Map.setOrigin(map1.getSize().x / 2, map1.getSize().y / 2);
    Map.setPosition(0, 178 * 16);

    //Player
    
    Idle.loadFromFile("idle.png");
    Idle2.loadFromFile("AblazeIdle.png");
    Player.setTexture(Idle);
    Player.setScale(player_scale, player_scale);
    Player.setOrigin(Idle.getSize().x / 2, Idle.getSize().y / 2);
    Player.setPosition(initial_position);

    // GUI

    gui.setSkillsTexture();
    gui.setPlayerInfoTexture();
    gui.setMonstersHPTexture();
    gui.setShopTexture();
    // walls

    gates[0].setPosition(-60, 6500);
    gates[1].setPosition(-60, 4640);
    gates[2].setPosition(-60, 2830);
    gates[3].setPosition(-60, 1540);
    gates[4].setPosition(-60, -275);
    gates[5].setPosition(-90, -2000);

    right_borders[0].setPosition(650, 6700);
    right_borders[1].setPosition(100, 6330);
    right_borders[2].setPosition(880, 5650);
    right_borders[3].setPosition(750, 5525);
    right_borders[4].setPosition(1050, 4750);
    right_borders[5].setPosition(100, 4450);
    right_borders[6].setPosition(675, 4250);
    right_borders[7].setPosition(805, 4025);
    right_borders[8].setPosition(925, 3100);
    right_borders[9].setPosition(800, 3100);
    right_borders[10].setPosition(100, 2625);
    right_borders[11].setPosition(1225, 2535);
    right_borders[12].setPosition(1420, 1800);
    right_borders[13].setPosition(1250, 1675);
    right_borders[14].setPosition(100, 1350);
    right_borders[15].setPosition(650, 940);
    right_borders[16].setPosition(650, -400);
    right_borders[17].setPosition(1075, 180);
    right_borders[18].setPosition(100, -1015);
    right_borders[19].setPosition(850, -2000);
    right_borders[20].setPosition(-120, 5940);
    right_borders[21].setPosition(-145, 5240);
    right_borders[22].setPosition(-145, 2160);
    right_borders[23].setPosition(-900, 2150);
    right_borders[24].setPosition(655, 2150);

    left_borders[0].setPosition(-990, 6700);
    left_borders[1].setPosition(-275, 6330);
    left_borders[2].setPosition(-1050, 5650);
    left_borders[3].setPosition(-925,5525);
    left_borders[4].setPosition(-1225,4750);
    left_borders[5].setPosition(-275,4450);
    left_borders[6].setPosition(-850,4250);
    left_borders[7].setPosition(-975,4025);
    left_borders[8].setPosition(-1085,3100);
    left_borders[9].setPosition(-1025, 3100);
    left_borders[10].setPosition(-275, 2625);
    left_borders[11].setPosition(-1500, 2525);
    left_borders[12].setPosition(-1600, 1800);
    left_borders[13].setPosition(-1450, 1675);
    left_borders[14].setPosition(-275, 1350);
    left_borders[15].setPosition(-850, 940);
    left_borders[16].setPosition(-1250, 180);
    left_borders[17].setPosition(-850, -400);
    left_borders[18].setPosition(-275, -1015);
    left_borders[19].setPosition(-1000, -2000);
    left_borders[20].setPosition(0, 5940);
    left_borders[21].setPosition(25, 5240);
    left_borders[22].setPosition(25, 2160);
    left_borders[23].setPosition(-825, 2150);
    left_borders[24].setPosition(755, 2150);
    
    up_borders[0].setPosition(-1000, 6500);
    up_borders[1].setPosition(150, 6500);
    up_borders[2].setPosition(-1000, 5600);
    up_borders[3].setPosition(800, 5600);
    up_borders[4].setPosition(-1150, 4700);
    up_borders[5].setPosition(150 , 4700);
    up_borders[6].setPosition(-1025 , 3100);
    up_borders[7].setPosition(825 , 3100);
    up_borders[8].setPosition(-875 , 2850);
    up_borders[9].setPosition(150 , 2850);
    up_borders[10].setPosition(-1450 , 1675);
    up_borders[11].setPosition(1250 , 1675);
    up_borders[12].setPosition(150 , 1575);
    up_borders[13].setPosition(-1275 , 1575);
    up_borders[14].setPosition(-1100 , 180);
    up_borders[15].setPosition(-850, -250);
    up_borders[16].setPosition( 150 , -250);
    up_borders[17].setPosition( 720 , 180);
    up_borders[18].setPosition( -1000 , -2100);
    up_borders[19].setPosition( -60 , 5950);
    up_borders[20].setPosition( -60 , 5300);
    up_borders[21].setPosition( -60 , 2175);
    up_borders[22].setPosition( -850 , 2160);
    up_borders[23].setPosition( 730 , 2160);

    down_borders[0].setPosition(-900, 7450);
    down_borders[1].setPosition(-970, 6300);
    down_borders[2].setPosition(130, 6300);
    down_borders[3].setPosition(-1000, 5500);
    down_borders[4].setPosition(775, 5500);
    down_borders[5].setPosition(-840, 4425);
    down_borders[6].setPosition(110, 4425);
    down_borders[7].setPosition(-875, 4225);
    down_borders[8].setPosition(700, 4225);
    down_borders[9].setPosition(825, 4025);
    down_borders[10].setPosition(-1000, 4025);
    down_borders[11].setPosition(-1275, 2600);
    down_borders[12].setPosition(150, 2600);
    down_borders[13].setPosition(-1525, 2500);
    down_borders[14].setPosition(1250, 2500);
    down_borders[15].setPosition(-1020, 1325);
    down_borders[16].setPosition(150, 1325);
    down_borders[17].setPosition(-1100, 900);
    down_borders[18].setPosition(720, 900);
    down_borders[19].setPosition(150, -1020);
    down_borders[20].setPosition(-850, -1020);
    down_borders[21].setPosition(-60, 5900);
    down_borders[22].setPosition(-60, 5150);
    down_borders[23].setPosition(-60, 2050);
    down_borders[24].setPosition(-850, 2100);
    down_borders[25].setPosition(730, 2100);

    setNormalMovesTexture();
    setAblazeMovesTexture();
    for (int i = 0; i < 8; i++) {
        walkAnimation[i].loadFromFile("walk/Walk" + to_string(i) + ".png");
    }
    for (int i = 0; i < 3; i++) {
        HitAnimation[i].loadFromFile("hit/Hit" + to_string(i) + ".png");
    }
    for (int i = 0; i < 3; i++) {
        DeathAnimation[i].loadFromFile("Dead/Dead" + to_string(i) + ".png");
    } 
    for (int i = 0; i < 4; i++) {
        upgrade_npc_idle[i].loadFromFile("Npc/upgrader/idle/" + to_string(i) +".png");
    }  
    for (int i = 0; i < 4; i++) {
        upgrade_npc_talk[i].loadFromFile("Npc/upgrader/talk/" + to_string(i) + ".png");
    }
     for (int i = 0; i < 11; i++) {
        trade_npc_idle[i].loadFromFile("Npc/trader/Idle2/Idle" + to_string(i) + ".png");
    }  
    for (int i = 0; i < 7; i++) {
        trade_npc_talk[i].loadFromFile("Npc/trader/Idle2/Idle" + to_string(i) + ".png");
    }
    for (int i = 0; i < 5; i++) {
        powerup[i].loadFromFile("Player effects/power up/powerup" + to_string(i) + ".png");
    }

    // Upgrade NPC
    UpgradeNPC.setTexture(upgrade_npc_idle[0]);
    UpgradeNPC.setOrigin(45, 45);
    UpgradeNPC.setPosition(300, 6700);
    UpgradeNPC.setScale(3,3);
    
    shadow1.setFillColor({ 10,10,10 });
    shadow1.setScale(2.25, 0.6666);
    shadow1.setRadius(30);
    shadow1.setPosition(285,6790);
    shadow1.setOrigin(15, 15);

    //Trade NPC
    TradeNPC.setTexture(trade_npc_idle[0]);
    TradeNPC.setOrigin(64, 64);
    TradeNPC.setPosition(-750, 6650);
    TradeNPC.setScale(2.5,2.5);

    shadow2.setFillColor({ 10,10,10 });
    shadow2.setScale(2,0.6666);
    shadow2.setRadius(20);
    shadow2.setPosition(-780, 6800);
    shadow2.setOrigin(10, 10);

    menu_opener.loadFromFile("Title card.mp3");
    death_sound.loadFromFile("Death Sound.mp3");
    game_music.loadFromFile("Game music.mp3");

    player_attack1.loadFromFile("Sound effects/unsheath_sword.mp3");
    player_attackX.loadFromFile("Sound effects/mixkit-X.wav");
    player_attackC.loadFromFile("Sound effects/mixkit-C.wav");
    player_attackV.loadFromFile("Sound effects/mixkit-V.wav");
    player_hurt.loadFromFile("Sound effects/Lit body hit.mp3");
    PlayerHurt.setBuffer(player_hurt);

    menu_sound.loadFromFile("Sound effects/interface.mp3");
    MenuSounds.setBuffer(menu_sound);

    game_font.loadFromFile("Ungai.ttf");

    go_next_text.setFont(game_font);
    go_next_text.setFillColor(Color{ 255,215,0 });
    go_next_text.setString("Press E to proceed to next level");
    go_next_text.setCharacterSize(30);
    go_next_text.setPosition(Player.getPosition().x-100, Player.getPosition().y + 100);
    
    upgrader_text.setFont(game_font);
    upgrader_text.setFillColor(Color{ 255,215,0 });
    upgrader_text.setString("Press E to meet 3m Mo7sen");
    upgrader_text.setCharacterSize(30);
    upgrader_text.setPosition(60, 6500);

    set_your_heart_text.setFont(game_font);
    set_your_heart_text.setFillColor(Color{ 255,215,0 });
    set_your_heart_text.setString("Press G to Set Your Heart Ablaze");
    set_your_heart_text.setCharacterSize(40);
    set_your_heart_text.setPosition(Player.getPosition().x-100, Player.getPosition().y + 100);

    //cutscene

    playerScene1.loadFromFile("cutscenePlayer.png");
    monsterScene1.loadFromFile("enemies/Rogue.png");
    textbox.loadFromFile("textbox.png");
    
    Skip_text.setFont(game_font);
    Skip_text.setFillColor(Color{ 255,215,0 });
    Skip_text.setString("Press TAB to proceed");
    Skip_text.setCharacterSize(30);
    Skip_text.setPosition(750, 900);
    
    CutScenetext.setFont(game_font);
    CutScenetext.setFillColor(Color::Black);
    CutScenetext.setString("I shall show you true TERROR");
    CutScenetext.setCharacterSize(40);
    CutScenetext.setPosition(775, 420);

    textboxSP.setTexture(textbox);
    textboxSM.setTexture(textbox);
    textboxSP.setScale(0.7, 0.7); 
    textboxSM.setScale(0.7, 0.7);
    textboxSP.setPosition(-100, 150);
    textboxSM.setPosition(500, 350);
    playerScene1S.setTexture(playerScene1);
    monsterScene1S.setScale(-16, 16);
    monsterScene1S.setPosition(2000, 465);
    playerScene1S.setScale(0.4, 0.4);
    playerScene1S.setPosition(100, 550);

}

void upgradeShop() {

    if (abs(Player.getPosition().x - UpgradeNPC.getPosition().x) < 280 && abs(Player.getPosition().y - UpgradeNPC.getPosition().y) < 190) {
        //cout << "  near!  ";
        shopNear = true;
        if (Keyboard::isKeyPressed(Keyboard::E) and !shopOpened ) {
            if (upgradetimer.getElapsedTime().asSeconds() > button_lag) {
                upgradetimer.restart();
                shopOpened = true;
            }
        }
        if (shopOpened) {
            if ((Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1)) and coinsCount >= damageUpCost and damageUp < 5) {
                if (upgradetimer.getElapsedTime().asSeconds() > button_lag) {
                    upgradetimer.restart();
                    damageUp++;
                    coinsCount -= damageUpCost;
                    damageUpCost += 10;
                }
            }
            if ((Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2)) and coinsCount >= healthUpCost and Max_Player_Health < 200)
            {
                if (upgradetimer.getElapsedTime().asSeconds() > button_lag) {
                    upgradetimer.restart();
                    Max_Player_Health+=20;
                    healthUp++;
                    Player_Health = Max_Player_Health;
                    coinsCount -= healthUpCost;
                    healthUpCost += 10;
                }
            }
            if ((Keyboard::isKeyPressed(Keyboard::Num3) || Keyboard::isKeyPressed(Keyboard::Numpad3)) and coinsCount >= cooldownUpCost and cooldownUp < 5)
            {
                if (upgradetimer.getElapsedTime().asSeconds() > button_lag) {
                    upgradetimer.restart();
                    cooldownUp++;
                    coinsCount -= cooldownUpCost;
                    cooldownUpCost += 10;
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::E)) {
                if (upgradetimer.getElapsedTime().asSeconds() > button_lag) {
                    shopOpened = false;
                    upgradetimer.restart();
                }
            }
        }
    }
    else
    {
        shopOpened = false;
        shopNear = false;
    }

}
void Draw() {
    window.clear();
    window.draw(Map);
    window.draw(shadow1);
    window.draw(shadow2);
    window.draw(UpgradeNPC);
    window.draw(TradeNPC);
    if (Ablaze)gui.drawUltEffect();
    if (!ispassing)window.draw(Player);
    ShowMonsters();       
    if (power_up) {
        Powerup.setScale(2,2);
        Powerup.setPosition(Player.getPosition().x-180, Player.getPosition().y-90);
        window.draw(Powerup);
    }
    if (go_next) {
        go_next_text.setPosition(Player.getPosition().x-350, Player.getPosition().y - 150);
        window.draw(go_next_text);
    } 
    if (AblazeReady) {
        set_your_heart_text.setPosition(Player.getPosition().x- 400, Player.getPosition().y - 300);
        window.draw(set_your_heart_text);
    }
   /* for (int i = 0; i < doors+1; i++) {
        window.draw(gates[i]);
    } 
    for(int i = 0; i < left_walls;i++){
        window.draw(left_borders[i]);
    }
    for(int i = 0; i < up_walls;i++){
        window.draw(up_borders[i]);
    }
    for(int i = 0; i < right_walls;i++)
      window.draw(right_borders[i]);

    for (int i = 0; i < down_walls; i++)
        window.draw(down_borders[i]);*/

    gui.drawGUI(window);    

    if (shopNear and !shopOpened) {
        window.draw(upgrader_text);
    }
    else if (shopOpened) gui.drawUpgradeMenu();
    window.display();
}


void playerMovement() { 
    if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::LShift))
    {
        velocity.y = -run_speed * playerdeltatime;
    }
    else if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::LShift))
    {
        velocity.y = run_speed * playerdeltatime;
    }
    else {
        velocity.y = 0;
    }

    if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::LShift))
    {
        Player.setScale(-player_scale, player_scale);
        velocity.x = -run_speed * playerdeltatime;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::LShift))
    {
        Player.setScale(player_scale, player_scale);
        velocity.x = run_speed * playerdeltatime;
    }
    else {
        velocity.x = 0;
    }

    checkCollisions();
    Player.move(velocity);

    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        velocity.y = -walk_speed * playerdeltatime;
    }
    else if (Keyboard::isKeyPressed(Keyboard::S))
    {
        velocity.y = walk_speed * playerdeltatime;
    }
    else {
        velocity.y = 0;
    }

    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        Player.setScale(-player_scale, player_scale);
        velocity.x = -walk_speed * playerdeltatime;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D))
    {
        Player.setScale(player_scale, player_scale);
        velocity.x = walk_speed * playerdeltatime;
    }
    else {
        velocity.x = 0;
    }

    checkCollisions();
    Player.move(velocity);

}

void trackView() {
    view.setCenter(Player.getPosition()); //update
}

void Switch_States() {
    // cooldown timer
    for (int i = 0; i < 5; i++) {
        cooldown[i] -= playerdeltatime;
        if (cooldown[i] <= 0)
            cooldown[i] = 0;
    }
    // state switch logic 
   
    if (!animation_running) {
        
        if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W)) && Keyboard::isKeyPressed(Keyboard::LShift)) {
            curr_state = run;
        }
        else if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W)) {
            curr_state = walk;
        }
        else {
            curr_state = idle;
        }

        if ((Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left)) && cooldown[0] == 0) {
            curr_state = base;       
             PlayerAttack.setBuffer(player_attack1);
             PlayerAttack.play();
             cooldown[0] = 1.5;
        } 
        if (Keyboard::isKeyPressed(Keyboard::X) && cooldown[1] == 0 ) {
            curr_state = xmove;
            PlayerAttack.setBuffer(player_attackX);
            PlayerAttack.play();
            cooldown[1] = (3 - cooldownUp*0.1) / cooldown_divider  ;
        }
        if (Keyboard::isKeyPressed(Keyboard::C) && cooldown[2] == 0 ) {
            curr_state = cmove;
            PlayerAttack.setBuffer(player_attackC);
            PlayerAttack.play();
            cooldown[2] = (6 - cooldownUp*0.3)/ cooldown_divider ;
        }
        if (Keyboard::isKeyPressed(Keyboard::V) && cooldown[3] == 0 ) {  
            curr_state = vmove;
            PlayerAttack.setBuffer(player_attackV);
            PlayerAttack.play();
            cooldown[3] = (9 - cooldownUp*0.5)/cooldown_divider ;
        }
        if (Mouse::isButtonPressed(Mouse::Right) && cooldown[4] == 0 ) {
            cooldown[4] = 3;
            isdashing = true;
        }
        
        if (ishit) {
            curr_state = hit;
            PlayerHurt.play();
        }
        if (Player_Health <= 0) {
            curr_state = dead;
        }
        
        // set Animation variables based on state 

        switch (curr_state)
        {
        case base:
            maximagecounter = 8;
            ImageCounter = 0; animation_running = true;
            break;
        case vmove:
            maximagecounter = 7;
            ImageCounter = 0; animation_running = true;
            break;
        case xmove:
            maximagecounter = 7;
            ImageCounter = 0; animation_running = true;
            break;
        case cmove:
            maximagecounter = 8;
            ImageCounter = 0; animation_running = true;
            break;
        case hit:
            maximagecounter = 3;
            ImageCounter = 0; animation_running = true;
            break;
        case dead:
            maximagecounter = 3;
            ImageCounter = 0; animation_running = true;
            break;
        }
    }
    
    // Animate 

    switch (curr_state) {
    case run:if (!Ablaze){maximagecounter = 8; Player.setTexture(RunAnimation[ImageCounter]); UpdateAnimationCounter(0.1 * animation_multiplier); break;}
            else { maximagecounter = 8; Player.setTexture(RunAnimationUlt[ImageCounter]); UpdateAnimationCounter(0.1 * animation_multiplier); break; }
                 
        case walk: maximagecounter = 8; Player.setTexture(walkAnimation[ImageCounter]); UpdateAnimationCounter(0.2); break;
        case idle: 
            if (Ablaze) Player.setTexture(Idle2);
            else Player.setTexture(Idle);
            UpdateAnimationCounter(0.1); break;
        case base: if (!Ablaze) { Player.setTexture(BaseAttack[ImageCounter]); UpdateAnimationCounter(0.08 * animation_multiplier); break; }//0.12
                 else { Player.setTexture(BaseAttackUlt[ImageCounter]); UpdateAnimationCounter(0.08 * animation_multiplier); break; }

        case vmove: if (!Ablaze) { Player.setTexture(Vmove[ImageCounter]); UpdateAnimationCounter(0.11 * animation_multiplier); break; }
                  else { Player.setTexture(VmoveUlt[ImageCounter]); UpdateAnimationCounter(0.11 * animation_multiplier); break; }

        case xmove: if (!Ablaze) { Player.setTexture(Xmove[ImageCounter]); UpdateAnimationCounter(0.1 * animation_multiplier); break; }
                  else { Player.setTexture(XmoveUlt[ImageCounter]); UpdateAnimationCounter(0.1 * animation_multiplier); break; }

        case cmove: if (!Ablaze) { Player.setTexture(Cmove[ImageCounter]); UpdateAnimationCounter(0.1 * animation_multiplier); break; }
                  else { Player.setTexture(CmoveUlt[ImageCounter]); UpdateAnimationCounter(0.1 * animation_multiplier); break; }

        case hit:Player.setTexture(HitAnimation[ImageCounter]); UpdateAnimationCounter(0.15); break;    
        case dead: Player.setTexture(DeathAnimation[ImageCounter]); UpdateAnimationCounter(0.1); break;
    }

    switch (upgrade_npc_state) {
        case npc_idle:UpgradeNPC.setTexture(upgrade_npc_idle[upgradeNPCImageCounter]); upgradeNpcAnimation(0.2);
            break;
        case npc_talk:UpgradeNPC.setTexture(upgrade_npc_talk[upgradeNPCImageCounter]); upgradeNpcAnimation(0.1);
            break;
    } 
    switch (trade_npc_state) {
        case npc_idle:TradeNPC.setTexture(trade_npc_idle[tradeNPCImageCounter]); tradeNpcAnimation(0.15);
            break;
        case npc_talk:TradeNPC.setTexture(trade_npc_talk[tradeNPCImageCounter]); tradeNpcAnimation(0.1);
            break;
    }

    if (power_up) {
        Powerup.setTexture(powerup[PowerUpImageCounter]); powerupAnimation(0.15);
    }
    if (Ablaze)is1 = true;
}

void UpdateAnimationCounter(float st) {
    AnimationCounter += playerdeltatime;
    if (AnimationCounter >= st)
    {
        AnimationCounter = 0;
        ImageCounter++;
        if (ImageCounter >= maximagecounter)
        {
            ishit = false;
            if (curr_state != state::dead) {
                if (animation_running) animation_running = false;
                ImageCounter = 0;
            }
            else {
                Player.setTexture(DeathAnimation[2]);
                isDead = true;
            }
        }
    }
}

void upgradeNpcAnimation(float st) {
    upgradeNPCcounter += playerdeltatime;
    if (upgradeNPCcounter >= st)
    {
        upgradeNPCcounter = 0;
        upgradeNPCImageCounter++;
        upgradeNPCImageCounter %= 4;
    }
}

void tradeNpcAnimation(float st) {
    tradeNPCcounter += playerdeltatime;
    if (tradeNPCcounter >= st) {
        tradeNPCcounter = 0;
        tradeNPCImageCounter++;
        tradeNPCImageCounter %= 11;
    }
}

void powerupAnimation(float st) {
    PowerUpcounter += playerdeltatime;
    if (PowerUpcounter >= st)
    {
        PowerUpcounter = 0;
        PowerUpImageCounter++;
        if (PowerUpImageCounter >= 5) {
            PowerUpImageCounter = 0;
            power_up = false;
        }
    }
}

void menu_handler() {
    while (true) {
        if (pagenum == 10)
        {
            while (window.isOpen())
            {
                Event event;
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        break;
                    }
                    if (MenuOpener.getStatus() != Sound::Playing && !openertrigger) {
                        MenuOpener.setVolume(80);
                        MenuOpener.setBuffer(menu_opener);
                        MenuOpener.play();
                        openertrigger = true;
                    }

                    if (event.type == Event::KeyPressed) {
                        if (event.key.code == Keyboard::Up) {
                            menu.MoveUp();
                            MenuSounds.play();
                        }
                        if (event.key.code == Keyboard::Down) {
                            menu.MoveDown();
                            MenuSounds.play();
                        }
                        if (event.key.code == Keyboard::Return) {
                            if (menu.pressed() == 0) {
                                if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                                    GameClock.restart();
                                    pagenum = 0;
                                }
                            }
                            if (menu.pressed() == 1) {
                                if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                                    GameClock.restart();
                                    pagenum = 1;
                                }
                            }
                            if (menu.pressed() == 2) {
                                if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                                    GameClock.restart();
                                    pagenum = -1;
                                }
                            }
                        }
                    }
                }
                window.clear();
                if (pagenum != 10) {
                    break;
                }
                window.draw(bg);
                menu.drawMenu(window);
                window.display();
            }
            if (pagenum == -1) {
                window.close();
                MenuOpener.stop();
                return;
            }
            if (pagenum == 0) {
                cutScenePlaying = true;
                Game_play(window);
            }
            if (pagenum == 1) {
                Credits_Menu(window);
            }
        }
    }
}

void Game_play(RenderWindow& window) {  
    while (window.isOpen()) {
        float elapsed = GameClock.restart().asSeconds();
        playerdeltatime = elapsed;
        Event Play;
        while (window.pollEvent(Play)) {
            if (Play.type == Event::Closed) {
                window.close();
            }
        }
        music_handler();
        if(cutScenePlaying) cutScene();
        if (!cutScenePlaying) {
            update();Draw();
        }
    }
}

void Credits_Draw() {
    window.clear();
    window.draw(Credits);
    window.display();
}

void Credits_Menu(RenderWindow& window) {
    while (window.isOpen()) {
        Event creditsMenu;
        while (window.pollEvent(creditsMenu)) {
            if (creditsMenu.type == Event::Closed) {
                window.close();
            }
        }
        Credits_Draw();
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            pagenum = 10;
            menu_handler();
        }
    }
}


void PauseMenuHandler(RenderWindow& window) {
    if (GameMusic.getStatus() == Sound::Playing) 
        GameMusic.pause();

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
     
        //point view at pause menu
        view.setCenter(960, 540); 
        window.setView(view);

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                pause.moveup();
                MenuSounds.play();
                GameClock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                pause.movedown();
                MenuSounds.play();
                GameClock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Enter) && pause.selectedp == 0) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                GameClock.restart();
                MenuSounds.play();
                GameMusic.play();
                break;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Enter) && pause.selectedp == 1) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                GameClock.restart();
                // options menu
            }
        }
        //restart
        if (Keyboard::isKeyPressed(Keyboard::Enter) && pause.selectedp == 2) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                MenuSounds.play();
                GameClock.restart();
                level = 1;
                cutScenePlaying = true;
                coinsCount = 40;
                Max_Player_Health = 100;
                Map.setTexture(map1);
                game_reset();
                break;
            }
        }
        //main menu
        if (Keyboard::isKeyPressed(Keyboard::Enter) && pause.selectedp == 3) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                MenuSounds.play();
                GameClock.restart();
                pagenum = 10;
                level = 1;
                cutScenePlaying = true;
                coinsCount = 40;
                Max_Player_Health = 100;
                Map.setTexture(map1);
                game_reset();
                menu_handler();
            }
        }

        window.clear();
        window.draw(pausemenu);
        pause.draw(window);
        window.display();
    }
}

void game_reset() {
    Player_Health = Max_Player_Health;
    curr_state = idle;
    DeathSound.stop();
    isDead = false;
    for (int i = 0; i < 5; i++) 
        cooldown[i] = 0;
    Player.setPosition(initial_position);
    Player.setScale(0.2, 0.2);
    openertrigger = false;
    map_opener_trigger = false;
    death_trigger = false;
    room_cleared = true;
    ResetMonsters();
    current_room = 0;
    current_wave = 0;
    AnimationCounter = 0;
    maximagecounter = 0;
    ImageCounter = 0;
    globalInt = 0;
    playerdeltatime = 0;
    pause.pausemenu[0].setFillColor(Color{ 255, 215, 0 });
    pause.selectedp = 0;
    for (int i = 1; i < 4; i++)
        pause.pausemenu[i].setFillColor(Color::White);

    AblazeCharge = 0;
    upgradeNPCcounter = 0;
    tradeNPCcounter = 0; 
    upgradeNPCImageCounter = 0;
    tradeNPCImageCounter = 0;
    AblazeReady = false;
    Ablaze = false;
    power_up = false;
}

void music_handler() {
    if (MenuOpener.getStatus() == Sound::Playing) MenuOpener.stop();
    if (GameMusic.getStatus() != Sound::Playing && !map_opener_trigger) {
        GameMusic.setVolume(5);
        GameMusic.setBuffer(game_music);
        GameMusic.play();
        map_opener_trigger = true;
    }
}

void death_handler() {

    lastX = Player.getPosition().x;
    lastY = Player.getPosition().y;
    GameOver game_over(1920, 1080, lastX, lastY);
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        DeathScreen.setPosition(lastX -675,lastY - 700 );
        Player.setTexture(DeathAnimation[2]);
        GameMusic.stop();


        if (DeathSound.getStatus() != Sound::Playing && !death_trigger) {
            DeathSound.setBuffer(death_sound);
            DeathSound.setVolume(50);
            DeathSound.play();
            death_trigger = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                game_over.moveup();
                MenuSounds.play();
                GameClock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                game_over.movedown();
                MenuSounds.play();
                GameClock.restart();
            }
        }

        // Restart
       
        if (Keyboard::isKeyPressed(Keyboard::Enter) && game_over.selectedp == 0) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                MenuSounds.play();
                GameClock.restart();
                level = 1;
                Map.setTexture(map1);
                game_reset();
                break;
            }
        }

        // retrun to Main menu

        if (Keyboard::isKeyPressed(Keyboard::Enter) && game_over.selectedp == 1) {
            if (GameClock.getElapsedTime().asSeconds() > button_lag) {
                MenuSounds.play();
                GameClock.restart();
                pagenum = 10;
                level = 1;
                Map.setTexture(map1);
                game_reset();
                view.setCenter(960, 540); //update
                window.setView(view);
                menu_handler();
            }
        }

        window.clear();
        window.draw(Map);
        if (!ispassing)
            window.draw(Player);

        ShowMonsters();
        window.draw(DeathScreen);
        game_over.draw(window);
        window.display();
    }
}

void check_room() {
    int initial = current_room;
    for (int i = 0; i < doors; i++) {
        if (Player.getPosition().y < gates[i].getPosition().y - 200) {
            current_room = max(current_room,i + 1);          
        }
    }
    if (current_room > initial){
        SetMonstersWave();
        room_cleared = false;
        if(current_room != 1)
            Player_Health += (15 + ((current_room - 1) * 5 ) * level );
        Player_Health = min(Player_Health, 2 * Max_Player_Health);
    }
}


void checkpause() {
    if (Keyboard::isKeyPressed(Keyboard::Escape) && !isDead) {
        if (pausetimer.getElapsedTime().asSeconds() > button_lag) {
            PauseMenuHandler(window);
            pausetimer.restart();
        }
    }
}


void dash() {

    if (isdashing) {
        if (dash_duration > 0) {
            run_speed = 1500;
            walk_speed = 1500;
            dash_duration -= playerdeltatime;
        }
        else {
            isdashing = false;
            dash_duration = 0.115;
            run_speed = 200;
            walk_speed = 100;
        }
    }  
}


void Go_Next() {

    go_next = true;
    if (Keyboard::isKeyPressed(Keyboard::E)) {
        
        game_reset();
        if (level == 1) {
            Map.setTexture(map2);
            level = 2;
            return;
        }
        else if (level == 2) {
            Map.setTexture(map3);
            level = 3;
            return;
        }
        else if(level == 3){
            Map.setTexture(map4);
            level = 4;
            return;
        }
        else if(level == 4) {
            Map.setTexture(map5);
            level = 5;
            return;
        }
    }   
}

void check_ablaze() {

    AblazeCharge = min(100, AblazeCharge);
    if (AblazeCharge >= 100)
        AblazeReady = true;
}

void set_your_heart_ablaze() {

    if (AblazeReady and Keyboard::isKeyPressed(Keyboard::G)) {
        for (int i = 0; i < 5; i++) {
            cooldown[i] = 0;
        }
        power_up = true;
        AblazeReady = false;
        Ablaze = true;
        AblazeCharge = 0;
        
    }
    if (Ablaze) {
        AblazeDuration -= playerdeltatime;
        run_speed = 400;
        animation_multiplier = 0.9;
        cooldown_divider = 2;
    }
    else {
        run_speed = 200;
        animation_multiplier = 1;
        cooldown_divider = 1;
    }

    if (AblazeDuration <= 0) {
        Ablaze = false;
        AblazeDuration = 25;
        AblazeCharge = 0;
    }
}

void camera_shake() {
    if (power_up)intensity = 35;
    if (fireUltFrame == 4  or power_up) {
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f;
        float offsetX = cos(angle) * intensity;
        float offsetY = sin(angle) * intensity;
        view.move(offsetX, offsetY);
        intensity *= 0.8f; // Damping effect  
    }
    else if (fireUltFrame == 3)intensity = 15;
}
void cutScene() {
    if (cutscene.getElapsedTime().asSeconds() > 0.2) { 
        if (monsterScene1SA.left == 150) { 
            monsterScene1SA.left = 0; 
        }
        else {
            monsterScene1SA.left += 50; 
        }
        monsterScene1S.setTextureRect(monsterScene1SA); 
        cutscene.restart();  
    }
    if (Keyboard::isKeyPressed(Keyboard::Tab))
        cutScenePlaying = false;

    window.clear(); 
    window.draw(textboxSM);
    window.draw(Skip_text);
    window.draw(monsterScene1S); 
    window.draw(playerScene1S); 
    window.draw(CutScenetext); 
    window.display(); 

}