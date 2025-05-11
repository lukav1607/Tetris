// ================================================================================================
// File: Game.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include <algorithm>
#include <cmath>
#include "Game.hpp"
#include "Utility.hpp"

Game::Game() :
	gameState(GameState::TitleScreen),
	isRunning(true),
	backgroundColor(sf::Color(17, 17, 18)),
	transparentDefaultOverlayColor(sf::Color(17, 17, 18, 150)),
	transparentOverlayAlpha(transparentDefaultOverlayColor.a),
	isPaused(false),
	titleFont("assets/fonts/seguibl.ttf"),
	textFont("assets/fonts/seguisb.ttf"),
	pauseTitle(textFont, "PAUSED", 80),
	pauseText(textFont, "Press ESC to continue", 40),
	titleScreenTitle(textFont, "TETRIS", 160),
	titleScreenText(textFont, "Press ENTER to start", 40),
	titleScreenAuthor(textFont, "Luka Vukorepa 2025", 30),
	titleScreenAuthorShadow(textFont, "Luka Vukorepa 2025", 30),
	gameOverTitle(textFont, "GAME OVER", 80),
	gameOverScore(textFont, "SCORE: 0", 50),
	gameOverText(textFont, "    Press ESC to exit\nor ENTER to continue", 40),
	hud(textFont),
	titleColorTransitionTime(2.f),
	score(0),
	level(0),
	totalLinesCleared(0),
	currentTetromino(generator.getNext()),
	nextTetromino(generator.getNext()),
	tetrominoMovementDelay(BASE_MOVEMENT_DELAY),
	tetrominoMovementTimer(0.f),
	isTetrominoWaitingForRotation(false),
	hasTetrominoCollidedDownward(false),
	areLinesFlashing(false),
	lineFlashDuration(0.4f),
	lineFlashInterval(0.1f),
	lineFlashTimer(0.f),
	lineFlashPhaseTimer(0.f),
	lineFlashPhaseSwitch(false),
	hasInitialDelayPassed(false),
	initialInputDelay(0.15f),
	heldInputDelay(0.05f),
	inputTimer(0.f),
	heldKey(HeldKey::None),
	heldKeyLastFrame(HeldKey::None),
	wasKeyJustPressed(false),
	canMoveAgain(true),
	music("assets/music/arcade-beat-323176.mp3"),
	baseMusicVolume(30.f),
	musicVolume(0.f)
{
	initializeWindow();
	music.setLooping(true);

	currentTetromino.updateDrawPosition();
	positionNextTetromino();

	hud.updateScore(score);
	hud.updateLevel(level);
	hud.updateLinesCleared(totalLinesCleared);

	nextTetrominoBox.setSize(sf::Vector2f(5 * Cell::SIZE, 4 * Cell::SIZE));
	nextTetrominoBox.setPosition(sf::Vector2f(WINDOW_WIDTH - 6 * Cell::SIZE, 2 * Cell::SIZE));
	nextTetrominoBox.setFillColor(Cell::EMPTY_COLOR);
	nextTetrominoBox.setOutlineThickness(2.5f);
	nextTetrominoBox.setOutlineColor(Grid::OUTLINE_COLOR);

	topBar.setSize(sf::Vector2f(WINDOW_WIDTH, Grid::OFFSET.y - 4));
	topBar.setPosition(sf::Vector2f(0.f, 0.f));
	topBar.setFillColor(backgroundColor);

	topBarBottomLine.setSize(sf::Vector2f(Grid::WIDTH * Cell::SIZE, 2.5f));
	topBarBottomLine.setPosition(sf::Vector2f(Grid::OFFSET.x, Grid::OFFSET.y - 3.5f));
	topBarBottomLine.setFillColor(Grid::OUTLINE_COLOR);

	transparentOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	transparentOverlay.setPosition(sf::Vector2f(0.f, 0.f));
	transparentOverlay.setFillColor(transparentDefaultOverlayColor);

	pauseTitle.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - pauseTitle.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f - pauseTitle.getGlobalBounds().size.y));
	pauseTitle.setFillColor(sf::Color(255, 245, 210));
	pauseTitle.setOutlineColor(sf::Color::White);
	pauseTitle.setOutlineThickness(0.5f);

	pauseText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - pauseText.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f + pauseTitle.getGlobalBounds().size.y));
	pauseText.setFillColor(sf::Color(255, 245, 210));
	pauseText.setOutlineColor(sf::Color::White);
	pauseText.setOutlineThickness(0.5f);

	titleScreenTitle.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - titleScreenTitle.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f - titleScreenTitle.getGlobalBounds().size.y * 1.5f));
	titleScreenTitle.setFillColor(sf::Color(255, 245, 210));
	titleScreenTitle.setOutlineColor(sf::Color::White);
	titleScreenTitle.setOutlineThickness(4.5f);

	titleScreenText.setOrigin(sf::Vector2f(titleScreenText.getGlobalBounds().size.x / 2.f, titleScreenText.getGlobalBounds().size.y / 2.f));
	titleScreenText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + titleScreenTitle.getGlobalBounds().size.y * 0.5f));
	titleScreenText.setFillColor(sf::Color(255, 245, 210));
	titleScreenText.setOutlineColor(sf::Color::White);
	titleScreenText.setOutlineThickness(0.5f);

	titleScreenAuthor.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - titleScreenAuthor.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT - titleScreenTitle.getGlobalBounds().size.y * 0.55f));
	titleScreenAuthor.setFillColor(sf::Color(255, 245, 210));
	titleScreenAuthor.setOutlineColor(sf::Color(40, 42, 50, 150));
	titleScreenAuthor.setOutlineThickness(2.f);

	titleScreenAuthorShadow.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - titleScreenAuthor.getGlobalBounds().size.x / 2.f + 2, WINDOW_HEIGHT - titleScreenTitle.getGlobalBounds().size.y * 0.55f + 2));
	titleScreenAuthorShadow.setFillColor(sf::Color(0, 0, 0, 200));

	gameOverTitle.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - gameOverTitle.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f - gameOverTitle.getGlobalBounds().size.y * 2.f));
	gameOverTitle.setFillColor(sf::Color(255, 245, 210));
	gameOverTitle.setOutlineColor(sf::Color::White);
	gameOverTitle.setOutlineThickness(0.5f);

	gameOverScore.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - gameOverScore.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f));
	gameOverScore.setFillColor(sf::Color(255, 245, 210));
	gameOverScore.setOutlineColor(sf::Color::White);
	gameOverScore.setOutlineThickness(0.5f);

	gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - gameOverText.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f + gameOverTitle.getGlobalBounds().size.y * 2.f));
	gameOverText.setFillColor(sf::Color(255, 245, 210));
	gameOverText.setOutlineColor(sf::Color::White);
	gameOverText.setOutlineThickness(0.5f);
}

int Game::run()
{
	const float FIXED_TIME_STEP = 1.f / 60.f; // Fixed time step per update
	sf::Clock clock;						  // Clock to measure time
	float timeSinceLastUpdate = 0.f;		  // Time accumulator for fixed timestep
	float interpolationFactor = 0.f;		  // Interpolation factor for rendering

	while (isRunning)
	{
		timeSinceLastUpdate += clock.restart().asSeconds();
		processInput();

		while (timeSinceLastUpdate >= FIXED_TIME_STEP)
		{
			update(FIXED_TIME_STEP);
			timeSinceLastUpdate -= FIXED_TIME_STEP;
		}

		interpolationFactor = timeSinceLastUpdate / FIXED_TIME_STEP;
		render();
	}
	return 0;
}

void Game::processInput()
{
	// Handle window events regardless of current game state
	while (const std::optional event = window.pollEvent())
		if (event->is<sf::Event::Closed>())
			isRunning = false;

	switch (gameState)
	{
	case GameState::TitleScreen:
		if (Utility::isKeyReleased(sf::Keyboard::Key::Enter))
		{
			soundManager.playSound(SoundManager::SoundID::GAME_START, 0.f, 1.f, 1.5f);
			gameState = GameState::InGame;
			resetGame();

			music.setVolume(0.f);
			music.play();
		}
		else if (Utility::isKeyReleased(sf::Keyboard::Key::Escape))
		{
			isRunning = false;
		}
		break;

	case GameState::InGame:
		// Pause and resume
		if (Utility::isKeyReleased(sf::Keyboard::Key::Escape) ||
			Utility::isKeyReleased(sf::Keyboard::Key::P))
		{
			isPaused = !isPaused;
			soundManager.playSound(SoundManager::SoundID::PAUSE, 0.f, 1.f, 0.15f);
		}

		// Prevent other input while paused
		if (isPaused) return;

		// Rotation
		if (Utility::isKeyReleased(sf::Keyboard::Key::Space) ||
			Utility::isKeyReleased(sf::Keyboard::Key::R) ||
			Utility::isKeyReleased(sf::Keyboard::Key::Up) ||
			Utility::isKeyReleased(sf::Keyboard::Key::W))
		{
			isTetrominoWaitingForRotation = true;
		}

		heldKeyLastFrame = heldKey;

		// Move left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			heldKey = HeldKey::Left;
			if (heldKeyLastFrame != heldKey && canMoveAgain)
			{
				currentTetromino.tryMove({ -1, 0 }, grid);
				currentTetromino.updateDrawPosition();
				canMoveAgain = false;
				inputTimer = 0.f;
				hasInitialDelayPassed = false;
			}
		}
		// Move right
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			heldKey = HeldKey::Right;
			if (heldKeyLastFrame != heldKey && canMoveAgain)
			{
				currentTetromino.tryMove({ 1, 0 }, grid);
				currentTetromino.updateDrawPosition();
				canMoveAgain = false;
				inputTimer = 0.f;
				hasInitialDelayPassed = false;
			}
		}
		// Move down
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			heldKey = HeldKey::Down;
		}
		else
		{
			heldKey = HeldKey::None;
			hasInitialDelayPassed = false;
		}
		break;

	case GameState::GameOver:
		if (Utility::isKeyReleased(sf::Keyboard::Key::Escape))
		{
			isRunning = false;
		}
		else if (Utility::isKeyReleased(sf::Keyboard::Key::Enter))
		{
			gameState = GameState::TitleScreen;
		}
		break;
	}
}

void Game::update(float fixedTimeStep)
{
	switch (gameState)
	{
	case GameState::TitleScreen:
		music.stop();

		updateTitleColor(fixedTimeStep);
		pulseTitleText(fixedTimeStep);
		break;

	case GameState::InGame:
		if (isPaused) return;

		canMoveAgain = true;

		if (music.getVolume() < baseMusicVolume)
		{
			musicVolume += 0.05f;
			if (music.getVolume() > baseMusicVolume)
				musicVolume = baseMusicVolume;

			music.setVolume(musicVolume);
		}

		updateTetrominoMovement(fixedTimeStep);

		if (isTetrominoWaitingForRotation)
		{
			currentTetromino.tryRotateCW(grid);
			currentTetromino.updateDrawPosition();
			isTetrominoWaitingForRotation = false;
		}
		
		if (hasTetrominoCollidedDownward)
		{
			lockTetromino();
			if (isGameOver())
			{
				gameState = GameState::GameOver;
				gameOverScore.setString("SCORE: " + std::to_string(score));
				gameOverScore.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - gameOverScore.getGlobalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f));
				soundManager.playSound(SoundManager::SoundID::GAME_OVER, 0.f, 1.f, 2.5f);
				return;
			}
			soundManager.playSound(SoundManager::SoundID::COLLISION, 0.25f, 3.5f, 0.3f);

			generateNextTetromino();
			hasTetrominoCollidedDownward = false;

			filledLines = grid.getFilledLines();
			if (!filledLines.empty())
				areLinesFlashing = true;
		}

		if (areLinesFlashing)
		{
			lineFlashTimer += fixedTimeStep;
			lineFlashPhaseTimer += fixedTimeStep;

			if (lineFlashPhaseTimer >= lineFlashInterval)
			{
				lineFlashPhaseTimer = 0.f;
				lineFlashPhaseSwitch = !lineFlashPhaseSwitch;

				for (const auto& line : filledLines)
					for (unsigned x = 0; x < Grid::WIDTH; ++x)
						grid.overwriteCellDrawColor({ x, line }, lineFlashPhaseSwitch ? sf::Color::Transparent : sf::Color::White);
			}

			if (lineFlashTimer >= lineFlashDuration)
			{
				areLinesFlashing = false;
			}
		}
		if (!areLinesFlashing && !filledLines.empty())
		{
			int previousLevel = level;

			lineFlashTimer = 0.f;
			lineFlashPhaseTimer = 0.f;

			score += getScoreWorth(filledLines.size());	
			totalLinesCleared += filledLines.size();
			level = totalLinesCleared / LINES_PER_LEVEL;
			tetrominoMovementDelay = std::max(MINIMUM_MOVEMENT_DELAY, BASE_MOVEMENT_DELAY - (level * MOVEMENT_DELAY_DECREASE));
			if (previousLevel != level)
				soundManager.playSoundAtPitch(SoundManager::SoundID::LEVEL_UP, 1.0f + static_cast<float>((level - 1) * 0.05f));

			hud.updateScore(score);
			hud.updateLevel(level);
			hud.updateLinesCleared(totalLinesCleared);

			grid.clearFilledLinesAndPushDown(filledLines);
			soundManager.playSoundAtPitch(SoundManager::SoundID::LINE_CLEAR, 1.0f + static_cast<float>((filledLines.size() - 1) * 0.25f), 1.f);

			filledLines.clear();
		}
		break;

	case GameState::GameOver:
		if (music.getVolume() > 0.f)
		{
			musicVolume -= 0.1f;
			if (music.getVolume() < 0.f)
				musicVolume = 0.f;

			music.setVolume(musicVolume);
		}

		if (transparentOverlayAlpha < 200)
		{
			transparentOverlayAlpha += 1;
			if (transparentOverlayAlpha > 200)
				transparentOverlayAlpha = 200;

			transparentOverlay.setFillColor(sf::Color
			(
				transparentDefaultOverlayColor.r,
				transparentDefaultOverlayColor.g,
				transparentDefaultOverlayColor.b,
				transparentOverlayAlpha
			));
		}
		break;
	}
	soundManager.cleanupSounds(fixedTimeStep, 10.f);
}

void Game::render()
{
	window.clear(backgroundColor);

	switch (gameState)
	{
	case GameState::TitleScreen:
		window.draw(titleScreenShapes);
		window.draw(titleScreenTitle);
		window.draw(titleScreenText);
		window.draw(titleScreenAuthorShadow);
		window.draw(titleScreenAuthor);
		break;

	case GameState::InGame:
	case GameState::GameOver:
		window.draw(topBar);
		window.draw(grid);
		window.draw(currentTetromino);
		window.draw(nextTetrominoBox);
		window.draw(nextTetromino);
		window.draw(topBar);
		window.draw(topBarBottomLine);
		window.draw(hud);

		if (isPaused)
		{
			window.draw(transparentOverlay);
			window.draw(pauseTitle);
			window.draw(pauseText);
		}
		if (isGameOver())
		{
			window.draw(transparentOverlay);
			window.draw(gameOverTitle);
			window.draw(gameOverScore);
			window.draw(gameOverText);
		}
		break;
	}

	window.display();
}

void Game::initializeWindow()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;
	window.create(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Tetris", sf::Style::Close, sf::State::Windowed, settings);
	window.setVerticalSyncEnabled(true);
}

void Game::resetGame()
{
	isPaused = false;
	transparentOverlay.setFillColor(transparentDefaultOverlayColor);
	transparentOverlayAlpha = transparentDefaultOverlayColor.a;
	score = 0;
	level = 0;
	totalLinesCleared = 0;
	hud.updateScore(score);
	hud.updateLevel(level);
	hud.updateLinesCleared(totalLinesCleared);
	filledLines.clear();
	tetrominoMovementDelay = BASE_MOVEMENT_DELAY;
	tetrominoMovementTimer = 0.f;
	isTetrominoWaitingForRotation = false;
	hasTetrominoCollidedDownward = false;
	areLinesFlashing = false;
	lineFlashTimer = 0.f;
	lineFlashPhaseTimer = 0.f;
	lineFlashPhaseSwitch = false;
	heldKeyLastFrame = HeldKey::None;
	heldKey = HeldKey::None;
	inputTimer = 0.f;
	wasKeyJustPressed = false;
	grid.reset();
	generator.reset();
	currentTetromino = generator.getNext();
	nextTetromino = generator.getNext();
	currentTetromino.updateDrawPosition();
	positionNextTetromino();
}

void Game::updateTitleColor(float fixedTimeStep)
{
	static float titleColorTimer = 0.f;
	static size_t currentColorIndex = 0U;

	titleColorTimer += fixedTimeStep;
	float t = std::min(titleColorTimer / titleColorTransitionTime, 1.f);

	sf::Color start = Tetromino::COLORS.at(currentColorIndex);
	sf::Color end = Tetromino::COLORS.at((currentColorIndex + 1) % Tetromino::COLORS.size());
	sf::Color interpolated = Utility::lerpColor(start, end, t);

	titleScreenTitle.setOutlineColor(interpolated); // assuming `titleText` is your sf::Text

	if (t >= 1.f)
	{
		titleColorTimer = 0.f;
		currentColorIndex = (currentColorIndex + 1) % Tetromino::COLORS.size();
	}
}

void Game::pulseTitleText(float fixedTimeStep)
{
	static float pulseTimer = 0.f;
	pulseTimer += fixedTimeStep;

	float scale = 1.f + 0.05f * std::sin(pulseTimer * 2.f);
	titleScreenText.setScale({ scale, scale });
}

int Game::getScoreWorth(unsigned linesCleared)
{
	if (linesCleared < 1 || linesCleared > 4)
		return 0;

	return baseScoresPerLine.at(static_cast<size_t>(linesCleared - 1)) * (level + 1);
}

bool Game::isGameOver() const
{
	for (unsigned x = 0; x < Grid::WIDTH; ++x)
	{
		if (grid.isCellFilled({ x, 0 }))
			return true;
	}
	return false;
}

void Game::updateTetrominoMovement(float fixedTimeStep)
{
	/* INPUT */
	// If held key was just changed or a key was just pressed
	/*if (heldKeyLastFrame != heldKey && heldKey != HeldKey::None)
	{
		inputTimer = 0.f;
		hasInitialDelayPassed = false;

		if (heldKey == HeldKey::Left)
			currentTetromino.tryMove({ -1, 0 }, grid);
		else if (heldKey == HeldKey::Right)
			currentTetromino.tryMove({ 1, 0 }, grid);
		else if (heldKey == HeldKey::Down)
			if (!currentTetromino.tryMove({ 0, 1 }, grid))
				hasTetrominoCollidedDownward = true;

		currentTetromino.updateDrawPosition();
	}*/
	// If a key is held down
	if (heldKey != HeldKey::None)
	{
		inputTimer += fixedTimeStep;

		// Check if the initial delay has passed and prevent movement until it has,
		// except for the Down key, which doesn't have an initial delay
		if (!hasInitialDelayPassed &&
			heldKey != HeldKey::Down)
		{
			if (inputTimer >= initialInputDelay)
			{
				inputTimer = 0.f;
				hasInitialDelayPassed = true;
			}
		}
		// Else, if the initial delay has passed, allow movement
		else
		{
			// Time between movements must be heldInputDelay seconds
			if (inputTimer >= heldInputDelay)
			{
				inputTimer = 0.f;

				if (heldKey == HeldKey::Left)
					currentTetromino.tryMove({ -1, 0 }, grid);
				else if (heldKey == HeldKey::Right)
					currentTetromino.tryMove({ 1, 0 }, grid);
				else if (heldKey == HeldKey::Down)
					if (!currentTetromino.tryMove({ 0, 1 }, grid))
						hasTetrominoCollidedDownward = true;

				currentTetromino.updateDrawPosition();
			}
		}
	}

	/* AUTOMATIC MOVEMENT */
	// Move the tetromino down automatically every tetrominoMovementDelay seconds but only if the Down key is not held
	if (heldKey != HeldKey::Down)
	{
		tetrominoMovementTimer += fixedTimeStep;
		if (tetrominoMovementTimer >= tetrominoMovementDelay)
		{
			tetrominoMovementTimer = 0.f;

			if (!currentTetromino.tryMove({ 0, 1 }, grid))
				hasTetrominoCollidedDownward = true;

			currentTetromino.updateDrawPosition();
		}
	}
}

void Game::lockTetromino()
{
	const auto& shape = currentTetromino.getShape();
	const auto& pos = currentTetromino.position;

	for (unsigned y = 0; y < 4; ++y)
	{
		for (unsigned x = 0; x < 4; ++x)
		{
			if (shape[y][x])
			{
				int gridX = static_cast<int>(pos.x) + x;
				int gridY = static_cast<int>(pos.y) + y;
				grid.fillCell(sf::Vector2u(gridX, gridY), currentTetromino.getColor());
			}
		}
	}
}

void Game::generateNextTetromino()
{
	currentTetromino = nextTetromino;
	currentTetromino.updateStartPosition();
	currentTetromino.updateDrawPosition();
	nextTetromino = generator.getNext();
	positionNextTetromino();
}

void Game::positionNextTetromino()
{
	if (nextTetromino.getType() == Tetromino::Type::I)
		nextTetromino.position = sf::Vector2f(11.5f, 1.5f);
	else if (nextTetromino.getType() == Tetromino::Type::O)
		nextTetromino.position = sf::Vector2f(12.5f, 2.f);
	else
		nextTetromino.position = sf::Vector2f(12.f, 2.f);

	nextTetromino.updateDrawPosition();
}