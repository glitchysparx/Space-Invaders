#pragma once

#include "../effects/GameEffects.h"

class VFXSystem;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object representating the enemy playing against the Player object
class Enemy : public Actor
{
public:
	Enemy() {}

	Enemy(int _ID)
	{
		ID = _ID;
		SetSize(_ID);
		SetBaseLocation(_ID);
	}

	void Construct(ResourceManager* resourceManager, VFXSystem* vfxSystem);

	int GetID() const { return ID; }
	void SetID(int _ID) { ID = _ID; }

	void SetSize(const int seed)
	{
		const float targetSize = CalculateSize(seed);
		size = Vector2D(targetSize);
	}

	void SetBaseLocation(const int seed)
	{
		baseLocation.x = static_cast<float>((seed % 10) * 60 + StartX);
		baseLocation.y = static_cast<float>((seed / 10) * 60 + StartY);
	}

	Vector2D GetBaseLocation() const { return baseLocation; }

	void OnFormationStep(Vector2D newFormationOffset);

	int GetReward() const { return killReward; }

	void Shoot();

protected:
	void OnTick(size_t frameID) override;
	void OnEndPlay(EndPlayReason reason) override;

	void UpdateLocation(size_t frameID);

	bool IsOrbitPhaseActive(int value) const
	{
		return ((value >> 6) & 0x7) == 0x7;
	}

	bool IsDivePhaseActive(int value) const
	{
		return ((value >> 8) & 0xf) == 0xf;
	}

	float CalculateSize(int seed) const
	{
		static constexpr float BaseSize = 25.f;
		static constexpr int SizeVariation = 17;
		return (BaseSize + (seed) % SizeVariation);
	}

	VFXSystem* vfxSystem = nullptr;
	SoundHandler soundExplosion = nullptr;
	SpriteAnimation animation;
	ResourceManager* resourceManager = nullptr;

	int ID = 0;

	Vector2D baseLocation = Vector2D(0.0f);
	Vector2D animOffset = Vector2D(0.0f);
	Vector2D formationOffset = Vector2D(0.0f);

	static constexpr float StartX = 120.0f;
	static constexpr float StartY = 130.0f;

	int killReward = 10;
};
