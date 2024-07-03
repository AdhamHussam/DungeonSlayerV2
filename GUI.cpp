#include "includes.h"
#include "GUI.h"
#include "globals.h"

using namespace std;
using namespace sf;

Texture skillframe1;
Clock frameClock;
float frameDelay = 0.11f;
void GUIskill::drawSkill(RenderWindow& window, int id) {
	font.loadFromFile("Ungai.ttf");
	Sprite border;
	border.setScale(Vector2f(3.5, 3.5));
	border.setPosition(pos.first, pos.second);
	border.setTexture(myFrame);
	window.draw(border);
	Text key;
	key.setFont(font);
	//key.setColor(color::White);
	if (id == 0) {
		key.setString("Left \n Click");
		key.setCharacterSize(14);
		key.setPosition(pos.first + 20, pos.second - 50);
	}
	else if (id == 1) key.setString(" X ");
	else if (id == 2) key.setString(" C ");
	else if (id == 3) key.setString(" V ");
	if (id != 0)key.setPosition(pos.first + 20, pos.second - 50);
	window.draw(key);
	RectangleShape avatar(sf::Vector2f(80, 80));
	avatar.setPosition(pos.first, pos.second);
	avatar.setTexture(&photo);
	window.draw(avatar);
}
void GUI::setSkillsTexture() {

	skills[0].photo.loadFromFile(R"(GUI\Base5.png)");

	skills[1].photo.loadFromFile(R"(GUI\Xmove4.png)");

	skills[2].photo.loadFromFile(R"(GUI\Cmove2.png)");

	skills[3].photo.loadFromFile(R"(GUI\Vmove3.png)");

	
}
void GUI::updateSkill() {
	string framename;
	framename = "GUI\\frame" + to_string(framenum);
	framename += ".png";
	string loadingname;
	loadingname = "GUI\\loading" + to_string(framenum);
	loadingname += ".png";
	loadingframe.loadFromFile(loadingname);
	skillframe.loadFromFile(framename);
	pair<int, int> firstposition = { Player.getPosition().x - 40 ,Player.getPosition().y + window.getSize().y / 3 + 50};

	//skills_background.setPosition(firstposition.first-240, firstposition.second);
	firstposition.second += 10;
	skills[0].pos = { firstposition.first - 180, firstposition.second };
	if (cooldown[0])
		skills[0].myFrame = loadingframe;
	else
		skills[0].myFrame = skillframe;

	skills[1].pos = { firstposition.first - 60, firstposition.second };
	if (cooldown[1])
		skills[1].myFrame = loadingframe;
	else
		skills[1].myFrame = skillframe;

	skills[2].pos = { firstposition.first + 60, firstposition.second };
	if (cooldown[2])
		skills[2].myFrame = loadingframe;
	else
		skills[2].myFrame = skillframe;

	skills[3].pos = { firstposition.first + 180, firstposition.second };
	if (cooldown[3])
		skills[3].myFrame = loadingframe;
	else
		skills[3].myFrame = skillframe;

	if (frameClock.getElapsedTime().asSeconds() > frameDelay) {
		framenum %= 4;
		framenum++;
		frameClock.restart();
	}

}

void GUI::setPlayerInfoTexture() {
	font.loadFromFile("Ungai.ttf");
	PlayerInfoTexture.loadFromFile(R"(GUI\playerInfo_1.png)");
	PlayerInfoUltTexture.loadFromFile(R"(GUI\playerInfo.png)");
	PlayerInfo.setTexture(PlayerInfoTexture);
	healthBarTexture.loadFromFile(R"(GUI\hp_bar.png)");
	healthBar.setTexture(healthBarTexture);
	staminaBarTexture.loadFromFile(R"(GUI\stamina_bar.png)");
	staminaBar.setTexture(staminaBarTexture);
	armorBarTexture.loadFromFile(R"(GUI\armor.png)");
	armorBar.setTexture(armorBarTexture);

	ultBarBackTexture.loadFromFile(R"(GUI\ult_bar_back.png)");
	ultBarBack.setTexture(ultBarBackTexture);
	ultBarTexture.loadFromFile(R"(GUI\ult_bar.png)");
	ultBar.setTexture(ultBarTexture);
	ultBarReadyTexture.loadFromFile(R"(GUI\ult_bar_full.png)");
	ultBar.setScale(4,13);
	ultBarBack.setScale(4,15);

	loadingEffectTexture.loadFromFile(R"(GUI\loading_effect.png)");
	loadingEffect.setTexture(loadingEffectTexture);
	loadingEffect.setScale(3.15,3.15);
	
	coinsTexture.loadFromFile(R"(Materials\items\Coins6mat\Coins6mat\coin53.png)");
	coins.setTexture(coinsTexture);
	coinsBackTexture.loadFromFile(R"(GUI\coins_back.png)");
	coinsBack.setTexture(coinsBackTexture);
	coinsCnt.setFont(font);
	upHeT.setFont(font);
	upAtT.setFont(font);
	upCoT.setFont(font);
	int size = 30;
	upHeT.setCharacterSize(size);
	upAtT.setCharacterSize(size);
	upCoT.setCharacterSize(size);

	upHeT.setString("Health              " + to_string(healthUpCost));
	upAtT.setString("Attack              " + to_string(damageUpCost));
	upCoT.setString("cooldown              " + to_string(cooldownUpCost));

}


void GUI::setShopTexture() {
	upgradeMenuTexture.loadFromFile(R"(GUI\UpgradeMenu.png)");
	upgradeMenu.setTexture(upgradeMenuTexture);
	upgradeMenu.setScale(1.5, 1.5);
}
void GUI::drawUpgradeMenu()
{
	Vector2f shopPosition = { Player.getPosition().x -670,Player.getPosition().y -300};
	Vector2f firstPosition = { Player.getPosition().x+330 ,Player.getPosition().y-40};
	upgradeMenu.setPosition(shopPosition);
	window.draw(upgradeMenu);

	upHeTexture.loadFromFile(R"(GUI\up)" + to_string(int(healthUp)) + R"(.png)");
	upAtTexture.loadFromFile(R"(GUI\up)" + to_string(int(damageUp)) + R"(.png)");
	upCoTexture.loadFromFile(R"(GUI\up)" + to_string(int(cooldownUp)) + R"(.png)");

	upHe.setTexture(upHeTexture);
	upHe.setScale(6, 4);
	upHe.setPosition(firstPosition);

	upAt.setTexture(upAtTexture);
	upAt.setScale(6, 4);
	upAt.setPosition(firstPosition.x, firstPosition.y-140);

	upCo.setTexture(upCoTexture);
	upCo.setScale(6, 4);
	upCo.setPosition(firstPosition.x, firstPosition.y + 140);

	firstPosition.x -= 150;

	upHeT.setPosition(firstPosition);
	upAtT.setPosition(firstPosition.x, firstPosition.y - 140);
	upCoT.setPosition(firstPosition.x, firstPosition.y + 140);

	upHeT.setFillColor(Color{ 70,30,0 });
	upAtT.setFillColor(Color{ 70,30,0 });
	upCoT.setFillColor(Color{ 70,30,0 });

	upHeT.setString(to_string(healthUpCost));
	upAtT.setString(to_string(damageUpCost));
	upCoT.setString(to_string(cooldownUpCost));

	window.draw(upHe);
	window.draw(upAt);
	window.draw(upCo);
	window.draw(upHeT);
	window.draw(upAtT);
	window.draw(upCoT);


}


void GUI::setMonstersHPTexture() {
	monsterHPBackTexture.loadFromFile(R"(GUI\monster_bar_back.png)");
	monsterHPBack.setTexture(monsterHPBackTexture);

	bossHPBackTexture.loadFromFile(R"(GUI\boss_empty.png)");
	bossHPBack.setTexture(bossHPBackTexture);

	monsterHPTexture.loadFromFile(R"(GUI\monster_bar.png)");
	monsterHP.setTexture(monsterHPTexture);
	bossHPTexture.loadFromFile(R"(GUI\boss_full.png)");
	bossHP.setTexture(bossHPTexture);

}

void GUI::DrawMonsterHP(Vector2f pos, float health, int origHealth, int xdif,int ydif)
{
	if (health <= 0)
		return;

	pos.x -= xdif;
	pos.y -= ydif;
	monsterHPBack.setPosition(pos);
	monsterHPBack.setScale(2,2);

	monsterHP.setPosition(pos);
	monsterHP.setScale(2,2);

	monsterHP.setTextureRect(IntRect(0, 0, monsterHPTexture.getSize().x * health/origHealth, monsterHPTexture.getSize().y));

	window.draw(monsterHPBack);
	window.draw(monsterHP);
}
void GUI::drawBODHP(Vector2f pos, float health) {
	
	if (health <= 0)
		return;

	pos.x -= 25;
	pos.y -= 60;
	monsterHPBack.setPosition(pos);
	monsterHPBack.setScale(2, 2);

	monsterHP.setPosition(pos);
	monsterHP.setScale(2, 2);

	monsterHP.setTextureRect(IntRect(0, 0, monsterHPTexture.getSize().x * health, monsterHPTexture.getSize().y));

	window.draw(monsterHPBack);
	window.draw(monsterHP);
}

void GUI::drawBossHP(float health, int max_health) {
	Vector2f pos = { Player.getPosition().x - bossHPTexture.getSize().x/2,
								Player.getPosition().y - window.getSize().y / 2 +40 };
	bossHPBack.setPosition(pos);
	bossHPBack.setScale(1, 1);

	bossHP.setPosition(pos);
	bossHP.setScale(1, 1);

	bossHP.setTextureRect(IntRect(0, 0, bossHPTexture.getSize().x * health/max_health, bossHPTexture.getSize().y));

	window.draw(bossHPBack);
	window.draw(bossHP);
}

void GUI::updatePlayerInfo(RenderWindow& window) {
	Vector2f infoPosition = { Player.getPosition().x - window.getSize().x/2-20,
								Player.getPosition().y - window.getSize().y / 2 -20};
	Vector2f ultPosition = { Player.getPosition().x + 870 ,Player.getPosition().y -220 };
	Vector2f coinsPosition = { Player.getPosition().x + window.getSize().x / 2 ,Player.getPosition().y - window.getSize().y / 2 };
	int helath_start = healthBarTexture.getSize().x/3;
	int health = (Player_Health > Max_Player_Health ? Max_Player_Health : Player_Health);
	int armor = Player_Health - Max_Player_Health;
	if (Ablaze)PlayerInfo.setTexture(PlayerInfoUltTexture);
	else PlayerInfo.setTexture(PlayerInfoTexture);
	PlayerInfo.setPosition(infoPosition);
	healthBar.setPosition(infoPosition.x + helath_start -  10, infoPosition.y);
	armorBar.setPosition(infoPosition.x + helath_start-10, infoPosition.y);
	staminaBar.setPosition(infoPosition.x + helath_start-10, infoPosition.y);

	staminaBar.setTextureRect(IntRect(helath_start - 10, 0,
		(3-cooldown[4]) / 3.0 * staminaBarTexture.getSize().x / 2, PlayerInfoTexture.getSize().y));

	healthBar.setTextureRect(IntRect(helath_start - 10, 0,
		 (float)health * 1.5 * helath_start / Max_Player_Health, PlayerInfoTexture.getSize().y));


	armorBar.setTextureRect(IntRect(helath_start - 10, 0,
		 (float)armor * 1.5 * helath_start / Max_Player_Health, PlayerInfoTexture.getSize().y));

	if (AblazeCharge == 100 or Ablaze)ultBar.setTexture(ultBarReadyTexture);
	else ultBar.setTexture(ultBarTexture);
	ultBarBack.setPosition(ultPosition);
	ultBar.setPosition(ultPosition.x, ultPosition.y + 30);

	if(!Ablaze)ultBar.setTextureRect(IntRect(0, 0, ultBarTexture.getSize().x, ultBarTexture.getSize().y*AblazeCharge/100.0));
	else ultBar.setTextureRect(IntRect(0, 0, ultBarTexture.getSize().x, ultBarTexture.getSize().y * AblazeDuration / 25.0));

	coins.setPosition(coinsPosition.x-180, coinsPosition.y+20);
	coins.setScale(0.6, 0.6);
	coinsBack.setPosition(coinsPosition.x - coinsBackTexture.getSize().x * 7-20, coinsPosition.y + coinsBackTexture.getSize().y * 4);
	coinsBack.setScale(7, 7);
	coinsCnt.setString(to_string((int)coinsCount));
	coinsCnt.setPosition(coinsPosition.x - 270, coinsPosition.y + 80);
	coinsCnt.setCharacterSize(30);

}

Clock fireUltTimer;
float fireUltLag = 1.0 / 12;
void GUI::drawUltEffect() {
	if (Ablaze and fireUltTimer.getElapsedTime().asSeconds() > fireUltLag) {
		
		fireUltTimer.restart();
		fireUltFrame++;
		fireUltFrame %= 12;
		fireUltTexture.loadFromFile(R"(GUI/fireUlt)" + to_string(fireUltFrame) + R"(.png)");
		fireUlt.setTexture(fireUltTexture);
		fireUlt.setScale(4, 4);
	}
	fireUlt.setPosition(Player.getPosition().x -150, Player.getPosition().y -120);
	window.draw(fireUlt);
}

void GUI::DrawloadingEffect(RenderWindow& window)
{

	Vector2f firstposition = { Player.getPosition().x - 35 ,Player.getPosition().y + window.getSize().y / 3 + 63 };
	float cooldowns[] = { 1.5, 3, 6, 9 };
	float posx[] = { firstposition.x - 180, firstposition.x - 60, firstposition.x + 60, firstposition.x + 180 };
	float posy = firstposition.y;
	for (int i = 0; i < 4; i++)
	{
		if (cooldown[i] > 0)
		{
			loadingEffect.setPosition(posx[i], posy);
			loadingEffect.setColor(Color(255, 255, 255, 255 * cooldown[i] / cooldowns[i]));
			/*loadingEffect.setTextureRect(IntRect(0, 0,
				 loadingEffectTexture.getSize().x * cooldown[i] / cooldowns[i],
				 loadingEffectTexture.getSize().y));*/
			window.draw(loadingEffect);
		}
	}

}
void GUI::drawGUI(RenderWindow& window) {
	updateSkill();
	updatePlayerInfo(window);
	window.draw(PlayerInfo);
	window.draw(healthBar);
	window.draw(armorBar);
	window.draw(staminaBar);
	window.draw(coinsBack);
	window.draw(coins);
	window.draw(coinsCnt);
	for (int i = 0; i < 4; i++)
	{
		skills[i].drawSkill(window, i);

	}
	window.draw(ultBarBack);
	window.draw(ultBar);
	
	
	DrawloadingEffect(window);

}



