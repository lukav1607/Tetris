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
#include "HUD.hpp"

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

	// Calculate score based on the number of lines just cleared and the current level and return it as an int
	int getScoreWorth(int linesCleared);

	bool isGameOver() const;

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
	sf::Color backgroundColor;

	sf::Font font;
	HUD hud;

	sf::RectangleShape topBar;
	sf::RectangleShape topBarBottomLine;

	const int LINES_PER_LEVEL = 10; // Number of lines to clear to level up
	int score;
	int level;
	int totalLinesCleared;

	// Score per line cleared in a single move
	const std::array<int, 4> baseScoresPerLine =
	{{
		{ 40 },  // 1 line cleared
		{ 100 }, // 2 lines cleared
		{ 300 }, // 3 lines cleared
		{ 1200 } // 4 lines cleared
	}};

	sf::RectangleShape nextTetrominoBox;

	Grid grid;

	TetrominoGenerator generator;
	Tetromino currentTetromino, nextTetromino;
	const float BASE_MOVEMENT_DELAY = 1.f; // Base delay between automatic tetromino movements
	const float MINIMUM_MOVEMENT_DELAY = 0.1f; // Minimum delay between automatic tetromino movements
	const float MOVEMENT_DELAY_DECREASE = 0.12f; // Movement delay decrease per level
	float tetrominoMovementDelay; // Delay between automatic tetromino movements
	float tetrominoMovementTimer; // Timer for automatic tetromino movement

	bool isTetrominoWaitingForRotation;
	bool hasTetrominoCollidedDownward;

	std::vector<unsigned> filledLines; // Lines that are filled and need to be cleared
	bool areLinesFlashing;
	float lineFlashDuration; // Duration for flashing filled lines
	float lineFlashInterval; // Interval between flashes
	float lineFlashTimer; // Timer for flashing filled lines
	float lineFlashPhaseTimer;
	bool lineFlashPhaseSwitch;

	enum class HeldKey
	{
		None,
		Left,
		Right,
		Down
	};
	HeldKey heldKey;
	HeldKey heldKeyLastFrame;
	bool wasKeyJustPressed;

	bool hasInitialDelayPassed;
	float initialInputDelay; // Delay before the first input is registered (not applied to Down key)
	float heldInputDelay; // Delay between inputs while a key is held down after the initial delay
	float inputTimer; // Timer for input delay
};