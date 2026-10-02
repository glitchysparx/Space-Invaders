#include "effectSystem.h"


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
	DrawSprite(sprite, targetLocation.x, targetLocation.y, frame.size.x/2, frame.size.y/2, frame.angle, frame.tint);
}



void VFXSystem::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager) { return; }

	this->resourceManager = resourceManager;
}

void VFXSystem::OnTick(size_t frameID)
{
	for (auto it = effects.begin(); it != effects.end(); )
	{
		Effect* effect = it->get();

		if (!effect->IsAlive())
		{
			it = effects.erase(it);
			continue;
		}

		effect->Tick(frameID);
		++it;
	}
}

void VFXSystem::Draw()
{
	if (!IsAlive()) { return; }

	for (const auto& effect : effects)
	{
		effect->Draw();
	}
}

void VFXSystem::PlayEffect(std::unique_ptr<Effect> effect)
{
	effect->Construct(resourceManager);
	effect->BeginPlay();
	effects.push_back(std::move(effect));
}

void VFXSystem::OnEndPlay(EndPlayReason reason)
{
	for (const auto& effect : effects)
	{
		effect->EndPlay(reason);
	}
}
