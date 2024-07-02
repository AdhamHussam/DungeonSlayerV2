#pragma once
#include "includes.h"

class PauseMenu
{
public:
    Text pausemenu[4];
    PauseMenu(float width, float height);
    void draw(RenderWindow& window);
    void moveup();
    void movedown();
    int selectedp; 
    void setSelected(int n);
    
    ~PauseMenu();
    
private:
    Font Pfont;
};

