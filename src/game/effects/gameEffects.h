#pragma once

#include "Effect.h"

class ExplosionEffect : public Effect
{
public:
	ExplosionEffect(Vector2D _location) : Effect(_location) {}

	void OnConstruct();
};