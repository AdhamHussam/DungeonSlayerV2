#include "Menu.h"
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;

Menu::Menu(float width, float height) {
	font.loadFromFile("Ungai.ttf");

	mainmenu[0].setFont(font);
	mainmenu[0].setFillColor(Color { 255,165,0 }); // coral : 255,127,80 ; gold : 255,215,0 ; orange : 255,165,0
	mainmenu[0].setString("Play");
	mainmenu[0].setCharacterSize(90);
	mainmenu[0].setPosition(Vector2f(100,400));

	mainmenu[1].setFont(font);
	mainmenu[1].setFillColor(Color::White);
	mainmenu[1].setString("Credits");
	mainmenu[1].setCharacterSize(90);
	mainmenu[1].setPosition(Vector2f(100,600));

	mainmenu[2].setFont(font);
	mainmenu[2].setFillColor(Color::White);
	mainmenu[2].setString("Exit");
	mainmenu[2].setCharacterSize(90);
	mainmenu[2].setPosition(Vector2f(100,800));

	selected = 0;
}


Menu::~Menu(){}

void Menu::setSelected(int n) {
	selected = n;
}


void Menu::drawMenu(RenderWindow& window) {
	for (int i = 0; i < 3; i++) {
		window.draw(mainmenu[i]);
	}
}

void Menu::MoveDown() {
	if (selected + 1 <= 3)
	{
		mainmenu[selected].setFillColor(Color::White);
		selected++;	
		if (selected == 3)
		{
			selected = 0;
		}
		{
			mainmenu[selected].setFillColor(Color{ 255,165,0 });
		}
	}

}

void Menu::MoveUp() { 
	if (selected - 1 >= -1)
	{
		mainmenu[selected].setFillColor(Color::White);
		selected--;
		if (selected == -1)
		{
			selected = 2;
		}
		mainmenu[selected].setFillColor(Color{ 255,165,0 });
	}
}