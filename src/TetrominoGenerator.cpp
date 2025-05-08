// ================================================================================================
// File: TetrominoGenerator.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <algorithm>
#include "TetrominoGenerator.hpp"

TetrominoGenerator::TetrominoGenerator()
{
	bag1.reserve(7);
	bag2.reserve(7);
	refillBag(bag1);
	refillBag(bag2);
}

Tetromino::Type TetrominoGenerator::getNext()
{
	if (bag1.empty())
	{
		bag1 = std::move(bag2);
		refillBag(bag2);
	}
	Tetromino::Type next = bag1.back();
	bag1.pop_back();
	return next;
}

void TetrominoGenerator::refillBag(Bag& bag)
{
	bag.clear();
	for (unsigned i = 0; i < 7; ++i)
	{
		bag.push_back(static_cast<Tetromino::Type>(i));
	}
	std::shuffle(bag.begin(), bag.end(), rng);
}