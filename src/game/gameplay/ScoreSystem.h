#pragma once

#include "../../framework/core/Object.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Object managing the score and containing the target achievable score value
class ScoreSystem : public Object
{
public:
	void Construct();

	void SetScore(const int value) { actualScore = value; }
	int GetScore() const { return actualScore; }
	void AddScore(const int value) { actualScore += value; }
	int GetHighScore() const { return highScore; }
	
protected:
	void OnEndPlay(EndPlayReason reason) override;

	int LoadHighScore();
	void SaveHighScore(const int value);

	int actualScore = 0;
	int highScore = 0;

	static constexpr char* highScoreFolder = "cache";
	static constexpr char* highScoreFile = "highscore.txt";
};
