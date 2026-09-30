#pragma once

#include "../gameplay/object.h"
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
class Effect : public Actor
{
public:
	Effect(Vector2D _location)
	{
		location = _location;
	}

	virtual void OnConstruct() = 0;

	void Construct(ResourceManager* resourceManager);

	void Draw() override;

	const AnimationFrame& GetActualFrame() const;

	bool IsAnimationFinished() const { return ticksSinceSpawn > animLength; }

protected:
	void OnTick(size_t frameID) override;

	std::vector<AnimationFrame> animationFrames;
	size_t currentFrameIndex = 0;

	size_t ticksSinceSpawn = 0;
	size_t animLength = 1;
	size_t uniformFrameDuration = 1;

	ResourceManager* resourceManager = nullptr;

private:
	void OnPostConstruct();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VFXSystem : public Object
{
public:
	void Construct(ResourceManager* resourceManager);

	void Draw();

	void PlayEffect(std::unique_ptr<Effect> effect);

	void EndPlay(EndPlayReason reason);

protected:
	void OnTick(size_t frameID) override;

	std::vector<std::unique_ptr<Effect>> effects;

	ResourceManager* resourceManager = nullptr;
};

