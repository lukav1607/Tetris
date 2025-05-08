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
	isPaused(false)
{
	initializeWindow();
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