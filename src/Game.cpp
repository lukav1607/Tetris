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
	hasInitialDelayPassed(false),
	initialInputDelay(0.2f),
	heldInputDelay(0.1f),
	inputTimer(0.f),
	heldKey(HeldKey::None),
	heldKeyLastFrame(HeldKey::None)
{
	initializeWindow();

	currentTetromino.updateDrawPosition();
	nextTetromino.position = sf::Vector2i(12, 2);
	nextTetromino.updateDrawPosition();
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
		if (Utility::isKeyReleased(sf::Keyboard::Key::Escape))
			isPaused = !isPaused;

		if (isPaused) return;

		if (Utility::isKeyReleased(sf::Keyboard::Key::Space))
		{
			currentTetromino = nextTetromino;
			currentTetromino.position = Tetromino::START_POSITION;
			nextTetromino = generator.getNext();

			currentTetromino.updateDrawPosition();
			nextTetromino.position = sf::Vector2i(12, 2);
			nextTetromino.updateDrawPosition();
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

		if (heldKey != heldKeyLastFrame)
		{
			inputTimer = 0.f;
			hasInitialDelayPassed = false;
		}

		if (heldKey != HeldKey::None)
		{
			inputTimer += fixedTimeStep;

			if (!hasInitialDelayPassed)
			{
				if (inputTimer >= initialInputDelay)
				{
					inputTimer = 0.f;
					hasInitialDelayPassed = true;
				}
			}
			else
			{
				if (inputTimer >= heldInputDelay)
				{
					inputTimer = 0.f;
					switch (heldKey)
					{
					case HeldKey::Left:
						currentTetromino.position.x -= 1;
						break;
					case HeldKey::Right:
						currentTetromino.position.x += 1;
						break;
					case HeldKey::Down:
						currentTetromino.position.y += 1;
						break;
					default:
						break;
					}
					currentTetromino.updateDrawPosition();
				}
			}
		}

		tetrominoMovementTimer += fixedTimeStep;

		if (tetrominoMovementTimer >= tetrominoMovementDelay)
		{
			tetrominoMovementTimer = 0.f;
			currentTetromino.position.y += 1;
			currentTetromino.updateDrawPosition();
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
	window.clear();

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