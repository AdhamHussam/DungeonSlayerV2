#pragma once
#include "includes.h"

struct GUIskill {
    Font font;
    Texture photo;
    Texture myFrame;
    pair<int, int> pos;

    void drawSkill(RenderWindow& window, int id);


    GUIskill() = default;
};


struct GUI {
    Font font;
    Texture skillframe;
    Texture loadingframe;
    Texture PlayerInfoUltTexture;
    Texture PlayerInfoTexture;
    Texture healthBarTexture;
    Texture armorBarTexture;
    Texture staminaBarTexture;
    Texture loadingEffectTexture;
    Texture monsterHPBackTexture;
    Texture monsterHPTexture;
    Texture bossHPBackTexture;
    Texture bossHPTexture;
    Texture ultBarTexture;
    Texture ultBarReadyTexture;
    Texture ultBarBackTexture;
    Texture coinsTexture;
    Texture coinsBackTexture;
    Texture upgradeMenuTexture;
    Texture upAtTexture;
    Texture upHeTexture;
    Texture upCoTexture;
    Texture fireUltTexture;

    GUIskill skills[4];
    
    Sprite PlayerInfo;
    Sprite healthBar;
    Sprite armorBar;
    Sprite staminaBar;
    Sprite loadingEffect;
    Sprite monsterHPBack;
    Sprite monsterHP;
    Sprite bossHPBack;
    Sprite bossHP;
    Sprite ultBarBack;
    Sprite ultBar;
    Sprite coins;
    Sprite coinsBack;
    Sprite upgradeMenu;
    Sprite upHe;
    Sprite upCo;
    Sprite upAt;
    Sprite fireUlt;
    Text upHeT;
    Text upCoT;
    Text upAtT;


    Text coinsCnt;
    int framenum = 1;

    RectangleShape skills_background;
    void setSkillsTexture();
    void updateSkill();
    void updatePlayerInfo(RenderWindow&window);
    void drawGUI(RenderWindow& window);
    void DrawloadingEffect(RenderWindow& window);
    void setPlayerInfoTexture();
    void setShopTexture();
    void drawUltEffect();
    void drawUpgradeMenu();
    void setMonstersHPTexture();
    void DrawMonsterHP(Vector2f pos, float health, int origHealth, int xdif, int ydif);
    void drawBODHP(Vector2f pos, float health);
    void drawBossHP(float health, int max_health);

    GUI() = default;
};