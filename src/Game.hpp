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
#include "TetrominoGenerator.hpp"

class Game
{
public:
	static constexpr unsigned WINDOW_WIDTH = 850U;
	static constexpr unsigned WINDOW_HEIGHT = 1100U;

	Game();
	int run();

private:
	void processInput();
	void update(float fixedTimeStep);
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

	sf::RenderWindow window;

	Grid grid;

	TetrominoGenerator generator;
	Tetromino currentTetromino, nextTetromino;
	float tetrominoMovementDelay;
	float tetrominoMovementTimer;

	enum class HeldKey
	{
		None,
		Left,
		Right,
		Down
	};
	HeldKey heldKey;
	HeldKey heldKeyLastFrame;

	bool hasInitialDelayPassed;
	float initialInputDelay;
	float heldInputDelay;
	float inputTimer;
};