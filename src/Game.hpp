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
	static constexpr unsigned WINDOW_WIDTH = 900U;
	static constexpr unsigned WINDOW_HEIGHT = 1100U;

	Game();
	int run();

private:
	void processInput();
	void update(float fixedTimeStep);
	void render();

	void initializeWindow();
	// Update the tetromino movement based on user input and automatic movement
	void updateTetrominoMovement(float fixedTimeStep);
	// Lock the tetromino in place
	void lockTetromino();
	// Generate the next tetromino
	void generateNextTetromino();
	// Position the next tetromino display to be centered on the right side of the screen
	void positionNextTetromino();

	enum class GameState
	{
		TitleScreen,
		InGame,
		GameOver
	};
	GameState gameState;
	bool isPaused;

	sf::RenderWindow window;

	sf::RectangleShape nextTetrominoBox;

	Grid grid;

	TetrominoGenerator generator;
	Tetromino currentTetromino, nextTetromino;
	float tetrominoMovementDelay; // Delay between automatic tetromino movements
	float tetrominoMovementTimer; // Timer for automatic tetromino movement

	bool isTetrominoWaitingForRotation;
	bool hasTetrominoCollidedDownward;

	float filledLineFlashDuration; // Duration for flashing filled lines
	float filledLineFlashInterval; // Interval between flashes
	float filledLineFlashTimer; // Timer for flashing filled lines

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
	float initialInputDelay; // Delay before the first input is registered (not applied to Down key)
	float heldInputDelay; // Delay between inputs while a key is held down after the initial delay
	float inputTimer; // Timer for input delay
};