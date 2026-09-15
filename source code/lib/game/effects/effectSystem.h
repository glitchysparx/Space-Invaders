#pragma once

#include "../gameplayObjects/object.h"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Data-only container for 2D animation frame
struct AnimationFrame
{
	AnimationFrame() {};

	AnimationFrame(char* _imagePath, Vector2D _size) :
		imagePath(_imagePath), size(_size), offset(Vector2D(0.f)), angle(0.f), tint(0xffffffff) {}

	AnimationFrame(char* _imagePath, Vector2D _size, Vector2D _offset, float _angle, DWORD _tint) :
		imagePath(_imagePath), size(_size), offset(_offset), angle(_angle), tint(_tint) {}

	// Graphics
	char* imagePath = nullptr;
	Vector2D size = Vector2D(0.f);
	Vector2D offset = Vector2D(0.f);
	DWORD tint = Color::White;
	float angle = 0.f;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 2D VFX animation
class Effect : public Object
{
public:
	Effect(Vector2D _location)
	{
		location = _location;
	}

	virtual void OnConstruct() = 0;

	void Construct(ResourceManager* _resourceManager) override;

	void Tick(size_t frameID) override;

	void Draw() override;

	const AnimationFrame& GetActualFrame() const;

	bool IsAnimationFinished() const { return ticksSinceSpawn > animLength; }

protected:
	std::vector<AnimationFrame> animationFrames;
	size_t currentFrameIndex = 0;

	size_t ticksSinceSpawn = 0;
	size_t animLength = 1;
	size_t uniformFrameDuration = 1;

private:
	void OnPostConstruct();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VFXSystem : public Object
{
public:
	void Tick(size_t frameID) override;

	void Draw() override;

	void PlayEffect(std::unique_ptr<Effect> effect);

	void EndPlay(EndPlayReason reason);

protected:
	std::vector<std::unique_ptr<Effect>> effects;
};

