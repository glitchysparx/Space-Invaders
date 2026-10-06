#pragma once

class GameStatus 
{
public:
	enum class Status
	{
		Ready,
		Playing,
		Victory,
		GameOver
	};

	Status Get() const { return statusCurrent; }
	Status GetPrevious() const { return statusPrevious; }

	void Set(Status value);

	void EndTick() 
	{
		bIsStatusChanged = false;
	}

	bool Is(Status value) const 
	{
		return statusCurrent == value;
	}

	bool WasChangedTo(Status value) const
	{
		return bIsStatusChanged && statusCurrent == value;
	}

protected:
	bool bIsStatusChanged = false;
	Status statusCurrent = Status::Ready;
	Status statusPrevious = Status::Ready;
};
