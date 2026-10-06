#include "VFXSystem.h"
#include "Effect.h"

VFXSystem::VFXSystem() = default;
VFXSystem::~VFXSystem() = default;
// default constructor and destructor are used because the class contains 
// std::array of std::unique_ptr<effect>, which will be automatically 
// managed by the compiler if "Effect.h" is included in the file

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
