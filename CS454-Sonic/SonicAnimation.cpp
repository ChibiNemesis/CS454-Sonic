#include "SonicAnimation.h"

SonicAnimation::SonicAnimation(std::string _id, int& _curr) : Animation(_id), CurrentFrame(_curr), MaxFrames(0)
{}

void SonicAnimation::SetCurrentFrame(int& curr)
{
    CurrentFrame = curr;
}

void SonicAnimation::SetMaxFrames(int _frames)
{
    MaxFrames = _frames;
}

void SonicAnimation::UpdateCurrentFrame()
{
    CurrentFrame++;
    if (CurrentFrame == MaxFrames)
        CurrentFrame = 0;
}

int SonicAnimation::GetMaxFrames()
{
    return MaxFrames;
}

int SonicAnimation::GetCurrentFrame()
{
    return CurrentFrame;
}
