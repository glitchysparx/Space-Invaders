#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Vector2D
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2D() = default;

	Vector2D(float value)
		: x(value), y(value) {
	}

	Vector2D(float xVal, float yVal)
		: x(xVal), y(yVal) {
	}

	Vector2D operator+(const Vector2D& other) const
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D operator-(const Vector2D& other) const
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D operator/(const float value) const
	{
		return Vector2D(x / value, y / value);
	}

	Vector2D operator*(const float value) const
	{
		return Vector2D(x * value, y * value);
	}
};