#pragma once


#include "../../framework/graphics/SpriteAnimation.h"
#include "../../framework/core/Object.h"

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
