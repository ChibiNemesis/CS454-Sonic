#pragma once
#include "Sprite.h"

class Spike :
    public Sprite
{
public:
    Spike();
    Spike(int _x, int _y, AnimationFilm* film, const std::string& _typeId = "");

    void Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const;

    void SetCollided(bool _collected);
    bool GetCollided();

private:
    bool IsCollided;

    void OnCollided();
};

