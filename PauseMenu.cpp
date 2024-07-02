#include "PauseMenu.h"
#include "includes.h"

PauseMenu::PauseMenu(float width, float height)
{
    Pfont.loadFromFile("Ungai.ttf");
    pausemenu[0].setFont(Pfont);
    pausemenu[0].setFillColor(Color { 255,215,0 });
    pausemenu[0].setString("Resume");
    pausemenu[0].setCharacterSize(90);
    pausemenu[0].setPosition(Vector2f(width /2.5- 27, height / 4));

    pausemenu[1].setFont(Pfont);
    pausemenu[1].setFillColor(Color::White);
    pausemenu[1].setString("Options");
    pausemenu[1].setCharacterSize(90);
    pausemenu[1].setPosition(Vector2f(width / 2.5 - 40, height / 4 + 200));

    pausemenu[2].setFont(Pfont);
    pausemenu[2].setFillColor(Color::White);
    pausemenu[2].setString("Restart");
    pausemenu[2].setCharacterSize(90);
    pausemenu[2].setPosition(Vector2f(width / 2.5 - 49, height / 4 + 400));

    pausemenu[3].setFont(Pfont);
    pausemenu[3].setFillColor(Color::White);
    pausemenu[3].setString("Main Menu");
    pausemenu[3].setCharacterSize(90);
    pausemenu[3].setPosition(Vector2f(width / 2.5 - 84, height / 4 + 600));
    selectedp = 0;
}

PauseMenu::~PauseMenu() {}

void PauseMenu::setSelected(int n)
{
    selectedp = n;
}

void PauseMenu::draw(RenderWindow& window )
{
    for (int i = 0; i < 4; i++) {
        window.draw(pausemenu[i]);
    }
}



void PauseMenu::movedown() {

    if (selectedp + 1 <= 4) {
        pausemenu[selectedp].setFillColor(Color::White);
        selectedp++;
        if (selectedp == 4)
            selectedp = 0;

        pausemenu[selectedp].setFillColor(Color{ 255,215,0 });


    }
}


void PauseMenu::moveup()
{
    if (selectedp - 1 >= -1) {
        pausemenu[selectedp].setFillColor(Color::White);
        selectedp--;
        if (selectedp == -1)
        {
            selectedp = 3;

        }
        pausemenu[selectedp].setFillColor(Color{ 255,215,0 });
    }
}