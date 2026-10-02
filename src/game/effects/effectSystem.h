#pragma once

#include "animation.h"

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

protected:
	void OnTick(size_t frameID) override;

	ResourceManager* resourceManager = nullptr;

	SpriteAnimation animation;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VFXSystem : public Object
{
public:
	void Construct(ResourceManager* resourceManager);

	void Draw();

	void PlayEffect(std::unique_ptr<Effect> effect);

protected:
	void OnTick(size_t frameID) override;
	void OnEndPlay(EndPlayReason reason) override;

	std::vector<std::unique_ptr<Effect>> effects;

	ResourceManager* resourceManager = nullptr;
};

