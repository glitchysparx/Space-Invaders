#pragma once


#include "../core/Vector2D.h"
#include "Colors.h"
#include <string>
#include <vector>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Data-only container for 2D animation frame
struct AnimationFrame
{
	AnimationFrame() {};

	AnimationFrame(std::string _imagePath, Vector2D _size) :
		imagePath(_imagePath), size(_size), offset(Vector2D(0.f)), angle(0.f), tint(0xffffffff) {
	}

	AnimationFrame(std::string _imagePath, Vector2D _size, Vector2D _offset, float _angle, DWORD _tint) :
		imagePath(_imagePath), size(_size), offset(_offset), angle(_angle), tint(_tint) {
	}

	// Graphics
    std::string imagePath;
	Vector2D size = Vector2D(0.f);
	Vector2D offset = Vector2D(0.f);
	DWORD tint = Color::White;
	float angle = 0.f;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpriteAnimation
{
public:
    void AddFrame(const AnimationFrame& frame)
    {
        frames.push_back(frame);
    }

    void Update();
    void AdvanceFrame();

    const AnimationFrame& GetCurrentFrame() const
    {
        assert(!frames.empty());
        return frames[currentFrame];
    }

    void Reset()
    {
        currentFrame = 0;
        ticksOnCurrentFrame = 0;
    }

    size_t GetFrameCount() const
    {
        return frames.size();
    }

    bool IsAnimationFinished() const;

	size_t GetAnimLength() const { return GetFrameCount() * uniformFrameDuration; }

	void SetUniformFrameDuration(size_t ticksPerFrame) { uniformFrameDuration = ticksPerFrame; }

	void SetLooping(bool shouldLoop) { looping = shouldLoop; }

	bool IsLooping() const { return looping; }

private:
    std::vector<AnimationFrame> frames;
    size_t currentFrame = 0;

    size_t uniformFrameDuration = 1; // TPF: ticks per frame

	bool looping = false;

    size_t ticksOnCurrentFrame = 0;
};
