#include "Effect.h"

void Effect::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager) { return; }
	this->resourceManager = resourceManager;

	OnConstruct();
}

void Effect::OnTick(size_t frameID)
{
	if (animation.IsAnimationFinished())
	{
		EndPlay(EndPlayReason::Destroyed);
		return;
	}

	animation.Update();
}

void Effect::Draw()
{
	auto& frame = animation.GetCurrentFrame();
	sprite = resourceManager->GetSprite(frame.imagePath);
	Vector2D targetLocation = location + frame.offset;
	DrawSprite(sprite, targetLocation.x, targetLocation.y, frame.size.x / 2, frame.size.y / 2, frame.angle, frame.tint);
}