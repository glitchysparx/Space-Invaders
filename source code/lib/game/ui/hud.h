#pragma once

#include "../gameplayObjects/object.h"
#include <array>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Renders the game title and processed its animation
class TitleRenderer : public Actor
{
public:
	void Construct(ResourceManager* resourceManager);

	void Tick(size_t frameID) override;

	void Draw() override;

	inline float GetLetterLocationX(const int letterID)
	{
		return letterID * LetterSpacing + StartX;
	}

protected:
	static constexpr int letterCount = 14;
	std::array<SpriteHandler, letterCount> letters{};

	static constexpr float LetterSpacing = 40.0f;
	static constexpr float StartX = 150.0f;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Text version of the score renderer
class ScoreRenderer : public Actor
{
public:
	void Construct();

	void Draw() override;

	void SetScore(const int value)
	{
		if (value < 0) { return; }
		score = value;
	}

	void SetHighScore(const int value)
	{
		if (value < 0) { return; }
		highScore = value;
	}

	void EndPlay(EndPlayReason reason) override;

protected:
	int score = 0;
	int highScore = 0;

	TextRenderer textRenderer;

	const  char* scoreTitle = "SCORE:";
	const  char* highScoreTitle = "HIGH SCORE:";
	const  int fontSize = 24;
	const float StartX1 = 100.0f;
	const float StartX2 = 500.0f;
	const float StartY = 80.0f;
	DWORD tint2 = Color::White; // used for high score
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Renders the screen frame. Screen size is described in gameWindow global object.
class ScreenFrameRenderer : public Actor
{
public:
	void Draw();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Renders the game final result and processed its animation
class FinalStatusRenderer : public Actor
{
public:
	void Construct();

	void Tick(size_t frameID) override;

	void Draw() override;

	void ReceiveStatus(GameStatus::Status value);

	void EndPlay(EndPlayReason reason) override;

protected:
	GameStatus::Status status = GameStatus::Status::Ready;
	TextRenderer textRenderer;
	char* textValue = nullptr;
	static constexpr int fontSizeDefault = 64;
	int fontSize = fontSizeDefault;

	void ExecuteTextAnimation(size_t frameID);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Main manager orchestrating all the ui elements and isolating them from the gameWorld object. It's controlled by gameWorld object.
class HUD : public Object
{
public:
	void Construct(ResourceManager* resourceManager);

	void Tick(size_t frameID) override;

	void Draw();

	void BeginPlay() override;

	void EndPlay(EndPlayReason reason) override;

	void SetStatus(GameStatus::Status status) { finalStatus.ReceiveStatus(status); }
	void SetScore(const int value)
	{
		score.SetScore(value);
	}

	void SetHighScore(const int value)
	{
		score.SetHighScore(value);
	}

	void DrawResult()
	{
		finalStatus.Draw();
	}

	void PrepareResult()
	{
		finalStatus.BeginPlay();
	}

protected:
	ScreenFrameRenderer screenFrame;
	TitleRenderer title;
	ScoreRenderer score;
	FinalStatusRenderer finalStatus;
};

