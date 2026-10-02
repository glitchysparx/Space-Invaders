#pragma once

#include "../utils/utils.h"
#include <vector>
#include <cassert>

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
	virtual ~Object() = 0;

	void Tick(size_t frameID);

	// Object is considered alive only at this step
	void BeginPlay();

	// Makes the object dead but leaves it still constructed
	void EndPlay(EndPlayReason reason);

	bool IsAlive() const { return bIsAlive; }

protected:
	virtual void OnTick(size_t frameID) {}
	virtual void OnBeginPlay() {}
	virtual void OnEndPlay(EndPlayReason reason) {}
	
	bool bIsAlive = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// A spatial object represented in the game world.
class Actor : public Object
{
public:
	~Actor() override = 0;

	// Render the sprite
	virtual void Draw();

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
	void OnEndPlay(EndPlayReason reason) override;

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
