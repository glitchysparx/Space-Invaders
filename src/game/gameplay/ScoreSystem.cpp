#include "ScoreSystem.h"
#include <algorithm>

void ScoreSystem::Construct()
{
	highScore = LoadHighScore();
}

void ScoreSystem::OnEndPlay(EndPlayReason reason)
{
	// To cache the actual score value if it is higher than previous score
	int lastHighScore = LoadHighScore();
	if (lastHighScore < actualScore)
	{
		SaveHighScore(actualScore);
	}
}

int ScoreSystem::LoadHighScore()
{
	std::string fullPath = std::string(highScoreFolder) + "/" + highScoreFile;

	std::ifstream file(fullPath);

	// Verify file exists
	if (!file.is_open()) { return 0; }

	int highScore = 0;
	file >> highScore;

	// Verify file contains valid data
	if (file.fail()) { return 0; }

	return highScore;
}

void ScoreSystem::SaveHighScore(const int value)
{
	if (value < 0) { return; }

	// 1. Automatically create the directory if it doesn't exist
	CreateDirectoryA(highScoreFolder, NULL);

	// 2. Combine folder and file name
	std::string fullPath = std::string(highScoreFolder) + "/" + highScoreFile;

	// 3. Now opening the file
	std::ofstream file(fullPath, std::ios::trunc);

	// Handle error
	if (!file.is_open()) { return; }

	file << value;
}
