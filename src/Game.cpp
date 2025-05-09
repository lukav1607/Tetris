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
#include "Game.hpp"
#include "Utility.hpp"

Game::Game() :
	gameState(GameState::InGame),
	backgroundColor(sf::Color(25, 26, 27)),
	isPaused(false),
	font("assets/fonts/seguisb.ttf"),
	hud(font),
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
	wasKeyJustPressed(false)
{
	initializeWindow();
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
}

int Game::run()
{
	const float FIXED_TIME_STEP = 1.f / 60.f; // Fixed time step per update
	sf::Clock clock;						  // Clock to measure time
	float timeSinceLastUpdate = 0.f;		  // Time accumulator for fixed timestep
	float interpolationFactor = 0.f;		  // Interpolation factor for rendering

	while (window.isOpen())
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
			window.close();

	switch (gameState)
	{
	{
	case GameState::TitleScreen:
		break;
	}
	{
	case GameState::InGame:
		// Pause and resume
		if (Utility::isKeyReleased(sf::Keyboard::Key::Escape) ||
			Utility::isKeyReleased(sf::Keyboard::Key::P))
		{
			isPaused = !isPaused;
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
		}
		// Move right
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			heldKey = HeldKey::Right;
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
	}
	{
	case GameState::GameOver:
		break;
	}
	}
}

void Game::update(float fixedTimeStep)
{
	switch (gameState)
	{
	case GameState::TitleScreen:
		break;

	case GameState::InGame:
		if (isPaused) return;

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
				std::cout << "Game Over!" << std::endl;
			}

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
			lineFlashTimer = 0.f;
			lineFlashPhaseTimer = 0.f;

			score += getScoreWorth(filledLines.size());	
			totalLinesCleared += filledLines.size();
			level = totalLinesCleared / LINES_PER_LEVEL;
			tetrominoMovementDelay = std::max(MINIMUM_MOVEMENT_DELAY, BASE_MOVEMENT_DELAY - (level * MOVEMENT_DELAY_DECREASE));

			hud.updateScore(score);
			hud.updateLevel(level);
			hud.updateLinesCleared(totalLinesCleared);
			std::cout << "Score: " << score << ", Level: " << level << ", Lines cleared: " << totalLinesCleared << std::endl;

			grid.clearFilledLinesAndPushDown(filledLines);
			filledLines.clear();
		}
		break;

	case GameState::GameOver:
		break;
	}
}

void Game::render()
{
	window.clear(backgroundColor);

	switch (gameState)
	{
	{
	case GameState::TitleScreen:
		break;
	}
	{
	case GameState::InGame:
		window.draw(topBar);
		window.draw(grid);
		window.draw(currentTetromino);
		window.draw(nextTetrominoBox);
		window.draw(nextTetromino);
		window.draw(topBar);
		window.draw(topBarBottomLine);
		window.draw(hud);
		break;
	}
	{
	case GameState::GameOver:
		break;
	}
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

int Game::getScoreWorth(int linesCleared)
{
	if (linesCleared < 1 || linesCleared > 4)
		return 0;

	return baseScoresPerLine.at(linesCleared - 1) * (level + 1);
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
	if (heldKeyLastFrame != heldKey && heldKey != HeldKey::None)
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
	}
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