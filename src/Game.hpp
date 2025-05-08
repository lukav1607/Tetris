// ================================================================================================
// File: Game.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Defines the Game class, which contains the main game loop and manages the game state.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Grid.hpp"

class Game
{
public:
	static constexpr unsigned WINDOW_WIDTH = 800U;
	static constexpr unsigned WINDOW_HEIGHT = 1200U;

	Game();
	void run();

private:
	void processInput();
	void update();
	void render();

	void initializeWindow();

	enum class GameState
	{
		TitleScreen,
		InGame,
		GameOver
	};
	GameState gameState;
	bool isPaused;

	sf::Clock clock;

	sf::RenderWindow window;
	Grid grid;
};