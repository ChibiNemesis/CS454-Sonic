#pragma once
#include "Sprite.h"
#include "CoinAnimation.h"
class Coin :
    public Sprite
{
public:
    Coin();
    Coin(int _x, int _y, AnimationFilm* film, const std::string& _typeId = "");

    void Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const;

    void SetCollected(bool _collected);
    bool GetCollected();

private:
    bool IsCollected;
    //CoinAnimation animation;

    void OnCollected();
};

