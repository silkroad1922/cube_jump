#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include <raylib.h>


class Button
{
    public:
        Button(Rectangle b):bounds(b){}
        void setTexture(const char* filename){texture = LoadTexture(filename);}
        void setFrame() {frame = {0.0f,0.0f,(float)texture.width,(float)texture.height};};
        Texture2D getTexture(){return texture;}
        Rectangle getFrame(){return frame;}
        Rectangle getBound(){return bounds;}
    private:
        Rectangle frame;
        Rectangle bounds;
        Texture2D texture;
};


#endif