// ================================================================================================
// File: Grid.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Grid.hpp"

Grid::Grid() :
	offset({ 50U, 50U })
{
	cells.resize(HEIGHT, std::vector<Cell>(WIDTH));
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			cells[y][x].shape.setPosition(sf::Vector2f(x * Cell::SIZE + offset.x, y * Cell::SIZE + offset.y));
		}
	}
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			target.draw(cells[y][x].shape);
		}
	}
}