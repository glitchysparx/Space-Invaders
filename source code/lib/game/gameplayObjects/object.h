#pragma once

#include "../utils/utils.h"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AABB
{
public:
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;

	static AABB Construct(float xLocation, float yLocation, float halfWidth, float halfHeight);

	// Function for trivial collision detection 
	static bool Intersects(const AABB& a, const AABB& b);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class EndPlayReason
{
	Unknown,
	Destroyed,
	GameShutdown
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Base class for all game elements
class Object
{
public:
	// Object is constracted but not alive
	virtual void Construct(ResourceManager* _resourceManager)
	{
		resourceManager = _resourceManager;
	}

	virtual void Tick(size_t frameID) = 0;
	virtual void Draw();

	// Object is considered alive only at this step
	virtual void BeginPlay() 
	{
		bIsAlive = true;
	};

	// Makes the object dead but leaves it still constructed
	virtual void EndPlay(EndPlayReason reason)
	{
		bIsAlive = false;
		bCollisionEnabled = false;
	}

	virtual ~Object() = default;

	bool IsAlive() const { return bIsAlive; }

	Vector2D GetLocation() const { return location; }
	void SetLocation(Vector2D newLocation) { location = newLocation; }

	Vector2D GetSize() const { return size; }
	void SetSize(Vector2D newSize) { size = newSize; }

	Vector2D GetCollisionSize() const { return collisionSize; }
	void SetCollisionSize(Vector2D newSize) { collisionSize = newSize; }

	bool IsCollisionEnabled() const { return bCollisionEnabled; }

	Vector2D GetCollisionLocation() const
	{
		return location + collisionOffset;
	}

	// Get object's collision bounds
	virtual AABB GetBounds() const;

	bool IsFullyOnScreen() const;

	bool IsFullyOnScreen(Vector2D targetLocation) const;

protected:
	ResourceManager* resourceManager = nullptr;

	bool bIsAlive = false;
	bool bCollisionEnabled = false;

	SpriteHandler sprite = nullptr;

	Vector2D location = Vector2D(0.f, 0.f);
	Vector2D size = Vector2D(32.f);
	Vector2D collisionSize = Vector2D(32.0f);
	Vector2D collisionOffset = Vector2D(0.0f, 0.0f);

	// Angle in radians
	float angle = 0.f;

	DWORD tint = Color::White;
};
