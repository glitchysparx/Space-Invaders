#include "animation.h"

void SpriteAnimation::Update()
{
    if (frames.empty() || IsAnimationFinished()) { return; }

    ++ticksOnCurrentFrame;

    if (ticksOnCurrentFrame < uniformFrameDuration) { return; }

    AdvanceFrame();
}

void SpriteAnimation::AdvanceFrame()
{
    if (frames.empty()) { return; }

    if (looping)
    {
        currentFrame = (currentFrame + 1) % GetFrameCount();
        ticksOnCurrentFrame = 0;
        return;
    }

    if (currentFrame < GetFrameCount() - 1)
    {
        ++currentFrame;
        ticksOnCurrentFrame = 0;
        return;
    }

    // Already on the last frame.
    // ticksOnCurrentFrame is kept as-is so IsAnimationFinished() can detect completion
}

bool SpriteAnimation::IsAnimationFinished() const
{
    if (looping) { return false; }

    return currentFrame == GetFrameCount() - 1 &&
        ticksOnCurrentFrame >= uniformFrameDuration;
}

