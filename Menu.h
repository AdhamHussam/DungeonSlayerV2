#pragma once
#include "includes.h"

class Menu
{
public:
	Text mainmenu[3];
	Menu(float width, float height);
	void drawMenu(RenderWindow& window);
	void MoveUp();
	void MoveDown();	
	void setSelected(int n);
	int pressed() {
		return selected;
	}
	 ~Menu();

private :
	int selected;
	Font font;
};

