// ================================================================================================
// File: TetrominoGenerator.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Defines the TetrominoGenerator class, which is responsible for generating random tetrominoes.
//              It holds 2 "bags" of tetrominoes, each containing 7 tetrominoes of different types in
//              a random order. The bag is used to fairly generate tetrominoes, ensuring that all
// 	            shapes are used before any are repeated. The generator can be used to get the next
// 	            tetromino in the sequence. If only the last piece is left in the first bag, the next
//              tetromino will be the first piece in the second bag. After the first bag is emptied,
//              the contents of the second bag become the first bag, and a new second bag is generated.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <random>
#include "Tetromino.hpp"

class TetrominoGenerator
{
public:
	TetrominoGenerator();

	Tetromino::Type getNext();

private:
	using Bag = std::vector<Tetromino::Type>;

	void refillBag(Bag& bag);

	Bag bag1, bag2;
	std::mt19937 rng{ std::random_device{}() };
};