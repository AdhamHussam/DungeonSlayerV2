#pragma once
#include "includes.h"


class GameOver
{
public:
    Text gameover[2];
    GameOver(float width, float height,float x , float y);
    void draw(RenderWindow& window);
    void moveup();
    void movedown();
    int selectedp;
    void setSelected(int n);

    ~GameOver();

private:
    Font Pfont;
};


