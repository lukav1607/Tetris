// ================================================================================================
// File: Grid.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Grid.hpp"

Grid::Grid()
{
	cells.resize(HEIGHT, std::vector<Cell>(WIDTH));
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			cells[y][x].shape.setSize(sf::Vector2f(Cell::SIZE, Cell::SIZE));
			cells[y][x].shape.setPosition(sf::Vector2f(y * Cell::SIZE, x * Cell::SIZE));
			cells[y][x].shape.setFillColor(sf::Color::Black);
			cells[y][x].shape.setOutlineThickness(1.f);
			cells[y][x].shape.setOutlineColor(sf::Color::White);
			cells[y][x].isFilled = false;
		}
	}
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			target.draw(cells[y][x].shape, states);
		}
	}
}