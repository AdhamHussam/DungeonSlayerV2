#include "GameOver.h"
#include "includes.h"

GameOver::GameOver(float width, float height, float x, float y)
{
    Pfont.loadFromFile("Ungai.ttf");
    gameover[0].setFont(Pfont);
    gameover[0].setFillColor(Color{ 139,0,0 });
    gameover[0].setString("Restart");
    gameover[0].setCharacterSize(70);
    gameover[0].setPosition(x-185, y+125) ;

    gameover[1].setFont(Pfont);
    gameover[1].setFillColor(Color::White);
    gameover[1].setString("Main Menu");
    gameover[1].setCharacterSize(70);
    gameover[1].setPosition(x-215, y + 250);

    selectedp = 0;
}

GameOver::~GameOver() {}

void GameOver::setSelected(int n)
{
    selectedp = n;
}

void GameOver::draw(RenderWindow& window)
{
    for (int i = 0; i < 2; i++) {
        window.draw(gameover[i]);
    }
}



void GameOver::movedown() {

    if (selectedp + 1 <= 2) {
        gameover[selectedp].setFillColor(Color::White);
        selectedp++;
        if (selectedp == 2)
            selectedp = 0;

        gameover[selectedp].setFillColor(Color{ 139,0,0 });
    }
}


void GameOver::moveup()
{
    if (selectedp - 1 >= -1) {
        gameover[selectedp].setFillColor(Color::White);
        selectedp--;
        if (selectedp == -1)
        {
            selectedp = 1;

        }

        gameover[selectedp].setFillColor(Color{ 139,0,0 });
    }
}