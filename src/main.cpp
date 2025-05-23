// ================================================================================================
// File: main.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Starting point for the project.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Game.hpp"

int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->run();
	return 0;
}