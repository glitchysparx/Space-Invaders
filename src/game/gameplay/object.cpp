#include "object.h"

AABB AABB::Construct(float xLocation, float yLocation, float halfWidth, float halfHeight)
{
	AABB box;
	box.left = xLocation - halfWidth;
	box.right = xLocation + halfWidth;
	box.top = yLocation - halfHeight;
	box.bottom = yLocation + halfHeight;
	return box;
}

bool AABB::Intersects(const AABB& a, const AABB& b)
{
	return !(a.right < b.left ||
		a.left > b.right ||
		a.bottom < b.top ||
		a.top > b.bottom);
}



Object::~Object() = default;

void Object::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }
	OnTick(frameID);
}

void Object::BeginPlay()
{
	if (IsAlive()) { return; }
	bIsAlive = true;
	OnBeginPlay();
}

void Object::EndPlay(EndPlayReason reason)
{
	if (!IsAlive()) { return; }
	bIsAlive = false;
	OnEndPlay(reason);
}



Actor::~Actor() = default;

void Actor::Draw()
{
	if (!IsAlive()) { return; }
	DrawSprite(sprite, location.x, location.y, size.x/2, size.y/2, angle, tint);
}

AABB Actor::GetBounds() const
{
	const Vector2D center = GetCollisionLocation();
	return AABB::Construct(center.x, center.y, collisionSize.x/2, collisionSize.y/2);
}

bool Actor::IsFullyOnScreen() const
{
	const Vector2D LU = location - size/2;
	const Vector2D RB = location + size/2;

	return IsVectorOnScreen(LU) && IsVectorOnScreen(RB);
}

bool Actor::IsFullyOnScreen(Vector2D targetLocation) const
{
	const Vector2D LU = targetLocation - size/2;
	const Vector2D RB = targetLocation + size/2;

	return IsVectorOnScreen(LU) && IsVectorOnScreen(RB);
}

void Actor::OnEndPlay(EndPlayReason reason)
{
	bCollisionEnabled = false;
}
