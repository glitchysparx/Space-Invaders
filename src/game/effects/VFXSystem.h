#pragma once


#include "../../framework/core/Object.h"
#include <memory>

class Effect;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VFXSystem : public Object
{
public:
	VFXSystem();
	~VFXSystem() override;

	void Construct(ResourceManager* resourceManager);

	void Draw();

	void PlayEffect(std::unique_ptr<Effect> effect);

protected:
	void OnTick(size_t frameID) override;
	void OnEndPlay(EndPlayReason reason) override;

	std::vector<std::unique_ptr<Effect>> effects;

	ResourceManager* resourceManager = nullptr;
};
