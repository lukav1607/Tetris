// ================================================================================================
// File: Game.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Game.hpp"
#include "Utility.hpp"

Game::Game() :
	gameState(GameState::InGame),
	isPaused(false),
	currentTetromino(generator.getNext()),
	nextTetromino(generator.getNext()),
	tetrominoMovementDelay(1.f),
	tetrominoMovementTimer(0.f),
	isTetrominoWaitingForRotation(false),
	hasTetrominoCollidedDownward(false),
	hasInitialDelayPassed(false),
	initialInputDelay(0.1f),
	heldInputDelay(0.05f),
	inputTimer(0.f),
	heldKey(HeldKey::None),
	heldKeyLastFrame(HeldKey::None)
{
	initializeWindow();
	currentTetromino.updateDrawPosition();
	positionNextTetromino();

	nextTetrominoBox.setSize(sf::Vector2f(5 * Cell::SIZE, 4 * Cell::SIZE));
	nextTetrominoBox.setPosition(sf::Vector2f(WINDOW_WIDTH - 6 * Cell::SIZE, 2 * Cell::SIZE));
	nextTetrominoBox.setFillColor(Cell::EMPTY_COLOR);
	nextTetrominoBox.setOutlineThickness(2.5f);
	nextTetrominoBox.setOutlineColor(Grid::OUTLINE_COLOR);
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
		if (Utility::isKeyReleased(sf::Keyboard::Key::Escape) ||
			Utility::isKeyReleased(sf::Keyboard::Key::P))
		{
			isPaused = !isPaused;
		}

		if (isPaused) return;

		if (Utility::isKeyReleased(sf::Keyboard::Key::Space) ||
			Utility::isKeyReleased(sf::Keyboard::Key::R) ||
			Utility::isKeyReleased(sf::Keyboard::Key::Up) ||
			Utility::isKeyReleased(sf::Keyboard::Key::W))
		{
			isTetrominoWaitingForRotation = true;
		}

		heldKeyLastFrame = heldKey;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			heldKey = HeldKey::Left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			heldKey = HeldKey::Right;
		}
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
	{
	case GameState::TitleScreen:
		break;
	}
	{
	case GameState::InGame:
		if (isPaused) return;

		updateTetrominoMovement(fixedTimeStep);

		if (isTetrominoWaitingForRotation)
		{
			currentTetromino.rotateCCW();
			if (!grid.isTetrominoAtValidPosition(currentTetromino))
			{
				currentTetromino.rotateCW();
			}
			currentTetromino.updateDrawPosition();
			isTetrominoWaitingForRotation = false;
		}
		
		if (hasTetrominoCollidedDownward)
		{
			lockTetromino();
			auto filledLines = grid.getFilledLines();
			grid.clearFilledLinesAndPushDown(filledLines);

			generateNextTetromino();
			hasTetrominoCollidedDownward = false;
		}

		break;
	}
	{
	case GameState::GameOver:
		break;
	}
	}
}

void Game::render()
{
	window.clear(sf::Color(23, 23, 24));

	switch (gameState)
	{
	{
	case GameState::TitleScreen:
		break;
	}
	{
	case GameState::InGame:
		window.draw(grid);
		window.draw(currentTetromino);
		window.draw(nextTetrominoBox);
		window.draw(nextTetromino);
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

void Game::updateTetrominoMovement(float fixedTimeStep)
{
	/* INPUT */
	// If held key is changed, reset the input timer
	if (heldKey != heldKeyLastFrame)
	{
		inputTimer = 0.f;
		hasInitialDelayPassed = false;
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
				switch (heldKey)
				{
				case HeldKey::Left:
					currentTetromino.position.x -= 1;
					if (!grid.isTetrominoAtValidPosition(currentTetromino))
					{
						currentTetromino.position.x += 1;
					}
					break;
				case HeldKey::Right:
					currentTetromino.position.x += 1;
					if (!grid.isTetrominoAtValidPosition(currentTetromino))
					{
						currentTetromino.position.x -= 1;
					}
					break;
				case HeldKey::Down:
					currentTetromino.position.y += 1;
					if (!grid.isTetrominoAtValidPosition(currentTetromino))
					{
						currentTetromino.position.y -= 1;
						hasTetrominoCollidedDownward = true;
					}
					break;
				default:
					break;
				}
				currentTetromino.updateDrawPosition();
			}
		}
	}

	/* AUTOMATIC MOVEMENT */
	// Move the tetromino down automatically every tetrominoMovementDelay seconds
	tetrominoMovementTimer += fixedTimeStep;
	if (tetrominoMovementTimer >= tetrominoMovementDelay)
	{
		tetrominoMovementTimer = 0.f;
		currentTetromino.position.y += 1;
		if (!grid.isTetrominoAtValidPosition(currentTetromino))
		{
			currentTetromino.position.y -= 1;
			hasTetrominoCollidedDownward = true;
		}
		currentTetromino.updateDrawPosition();
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
	currentTetromino.position = Tetromino::START_POSITION;
	currentTetromino.updateDrawPosition();
	nextTetromino = generator.getNext();
	positionNextTetromino();
}

void Game::positionNextTetromino()
{
	if (nextTetromino.getType() == Tetromino::Type::I)
		nextTetromino.position = sf::Vector2f(11.5f, 2.5f);
	else if (nextTetromino.getType() == Tetromino::Type::O)
		nextTetromino.position = sf::Vector2f(12.5f, 2.f);
	else
		nextTetromino.position = sf::Vector2f(12.f, 2.f);

	nextTetromino.updateDrawPosition();
}