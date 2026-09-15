#include "effectSystem.h"


void Effect::Construct(ResourceManager* _resourceManager)
{
	if (resourceManager || !_resourceManager) { return; }
	Object::Construct(_resourceManager);

	OnConstruct();
	OnPostConstruct();
}

void Effect::Tick(size_t frameID)
{
	if (IsAnimationFinished())
	{
		++ticksSinceSpawn;
		return;
	}

	if (ticksSinceSpawn > uniformFrameDuration * (currentFrameIndex + 1))
	{
		++currentFrameIndex;
	}

	++ticksSinceSpawn;
}

void Effect::Draw()
{
	auto& frame = GetActualFrame();
	sprite = resourceManager->GetSprite(frame.imagePath);
	Vector2D targetLocation = location + frame.offset;
	DrawSprite(sprite, targetLocation.x, targetLocation.y, frame.size.x, frame.size.y, frame.angle, frame.tint);
}

const AnimationFrame& Effect::GetActualFrame() const
{
	assert(!animationFrames.empty());
	assert(currentFrameIndex < animationFrames.size());

	return animationFrames[currentFrameIndex];
}

void Effect::OnPostConstruct()
{
	animLength = animationFrames.size() * uniformFrameDuration;
}



void VFXSystem::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }

	for (auto it = effects.begin(); it != effects.end(); )
	{
		Effect* effect = it->get();

		if (!effect->IsAlive())
		{
			it = effects.erase(it);
			continue;
		}

		if (effect->IsAnimationFinished())
		{
			effect->EndPlay(EndPlayReason::Destroyed);
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

void VFXSystem::EndPlay(EndPlayReason reason)
{
	if (!IsAlive()) { return; }

	for (const auto& effect : effects)
	{
		effect->EndPlay(reason);
	}

	Object::EndPlay(reason);
}
