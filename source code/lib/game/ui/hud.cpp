#include "hud.h"


void ScreenFrameRenderer::Draw()
{
	DrawLine(0.f, 0.f, 0.f, gameWindow.height, tint); // left vertical line
	DrawLine(gameWindow.width - 1.f, 0.f, gameWindow.width - 1.f, gameWindow.height, tint); // right vertical line
}



void FinalStatusRenderer::Construct()
{
	textRenderer.FindFont(fontSize);
	location = Vector2D(0.f, 280.f);
}

void FinalStatusRenderer::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }

	ExecuteTextAnimation(frameID);
}

void FinalStatusRenderer::Draw()
{
	textRenderer.StartTextBatch();

	textRenderer.DrawSomeText(location.x, location.y, fontSize, tint, false, textValue);

	textRenderer.EndTextBatch();
}

void FinalStatusRenderer::ReceiveStatus(GameStatus::Status value)
{
	if (status == value) { return; }

	status = value;

	switch (status)
	{
	case GameStatus::Status::Victory:
		textValue = "VICTORY!";
		tint = Color::Green;
		location.x = 250.f;
		break;
	case GameStatus::Status::GameOver:
		textValue = "GAME OVER!";
		tint = Color::Red;
		location.x = 220.f;
		break;
	default:
		textValue = "unhandled status...";
		break;
	}
}

void FinalStatusRenderer::EndPlay(EndPlayReason reason)
{
	textRenderer.ReleaseFonts();
	Object::EndPlay(reason);
}

void FinalStatusRenderer::ExecuteTextAnimation(size_t frameID)
{
	const float fps = 60.f;
	const float timeSeconds = static_cast<float>(frameID) / fps;

	const float speed = 10.f; // animation speed
	const float amplitude = 0.02f; // 2% size change

	// Sinusoid font scale animation
	const float multiplier = 1.f + amplitude * sinf(timeSeconds * speed);
	fontSize = fontSizeDefault * multiplier;

	// To center the text on the screen depends on the fontSize
	const float tunedConstant = 0.55f; // It is needed because unfortunately fontSize tells almost nothing about size in pixels. This constants is tuned specifically to the used font
	location.x = gameWindow.width / 2 - (strlen(textValue) * fontSize / 2 * tunedConstant);
}



void HUD::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager) { return; }

	title.Construct(resourceManager);
	score.Construct();
	finalStatus.Construct();
}

void HUD::Tick(size_t frameID)
{
	if (!IsAlive()) { return; }

	title.Tick(frameID);
	screenFrame.Tick(frameID);
	score.Tick(frameID);
	finalStatus.Tick(frameID);
}

void HUD::Draw()
{
	if (!IsAlive()) { return; }

	title.Draw();
	screenFrame.Draw();
	score.Draw();
}

void HUD::BeginPlay()
{
	if (IsAlive()) { return; }
	Object::BeginPlay();

	title.BeginPlay();
	screenFrame.BeginPlay();
	score.BeginPlay();
}

void HUD::EndPlay(EndPlayReason reason)
{
	title.EndPlay(reason);
	screenFrame.EndPlay(reason);
	score.EndPlay(reason);
	finalStatus.EndPlay(reason);
	Object::EndPlay(reason);
}



void TitleRenderer::Construct(ResourceManager* resourceManager)
{
	if (!resourceManager) { return; }

	letters[0] = resourceManager->GetSprite("gfx/textSprites/slet.png");
	letters[1] = resourceManager->GetSprite("gfx/textSprites/plet.png");
	letters[2] = resourceManager->GetSprite("gfx/textSprites/alet.png");
	letters[3] = resourceManager->GetSprite("gfx/textSprites/clet.png");
	letters[4] = resourceManager->GetSprite("gfx/textSprites/elet.png");
	letters[5] = nullptr; // space
	letters[6] = resourceManager->GetSprite("gfx/textSprites/ilet.png");
	letters[7] = resourceManager->GetSprite("gfx/textSprites/nlet.png");
	letters[8] = resourceManager->GetSprite("gfx/textSprites/vlet.png");
	letters[9] = resourceManager->GetSprite("gfx/textSprites/alet.png");
	letters[10] = resourceManager->GetSprite("gfx/textSprites/dlet.png");
	letters[11] = resourceManager->GetSprite("gfx/textSprites/elet.png");
	letters[12] = resourceManager->GetSprite("gfx/textSprites/rlet.png");
	letters[13] = resourceManager->GetSprite("gfx/textSprites/slet.png");

	location.y = 30.f;
	size = Vector2D(32.f);
}

void TitleRenderer::Tick(size_t frameID)
{
	angle = sin(frameID * 0.1) * 0.1;
}

void TitleRenderer::Draw()
{
	for (int n = 0; n < letterCount; ++n)
	{
		if (letters[n] != nullptr)
		{
			DrawSprite(letters[n], GetLetterLocationX(n), location.y, size.x/2, size.y/2, angle);
		}
	}
}




void ScoreRenderer::Construct()
{
	textRenderer.FindFont(fontSize);
	location = Vector2D(0.f, StartY);
	tint = Color::Yellow;
	tint2 = Color::Red;
}

void ScoreRenderer::Draw()
{
	textRenderer.StartTextBatch();

	// Draw actual score
	textRenderer.DrawSomeText(StartX1, location.y, fontSize, tint, false, "%s %d", scoreTitle, score);

	// Draw high score
	textRenderer.DrawSomeText(StartX2, location.y, fontSize, tint2, false, "%s %d", highScoreTitle, highScore);

	textRenderer.EndTextBatch();
}

void ScoreRenderer::EndPlay(EndPlayReason reason)
{
	textRenderer.ReleaseFonts();
	Object::EndPlay(reason);
}

