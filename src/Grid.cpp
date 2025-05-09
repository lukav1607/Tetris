// ================================================================================================
// File: Grid.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "Grid.hpp"

Grid::Grid()
{
	cells.resize(HEIGHT, std::vector<Cell>(WIDTH));
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			cells[y][x].drawable.setPosition(sf::Vector2f(x * Cell::SIZE + OFFSET.x, y * Cell::SIZE + OFFSET.y));
		}
	}

	outline.setSize(sf::Vector2f(WIDTH * Cell::SIZE + 2, HEIGHT * Cell::SIZE + 2));
	outline.setPosition({ OFFSET.x - 1, OFFSET.y - 1 });
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(2.5f);
	outline.setOutlineColor(OUTLINE_COLOR);
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			target.draw(cells[y][x].drawable);
		}
	}
	target.draw(outline);
}

std::vector<unsigned> Grid::getFilledLines()
{
	std::vector<unsigned> filledLines;
	for (unsigned y = 0; y < HEIGHT; ++y)
	{
		bool isFilled = true;
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			if (!cells[y][x].isFilled)
			{
				isFilled = false;
				break;
			}
		}
		if (isFilled)
			filledLines.push_back(y);
	}
	return filledLines;
}

void Grid::clearFilledLinesAndPushDown(const std::vector<unsigned>& filledLines)
{
	for (const auto& line : filledLines)
	{
		// Clear every cell in the filled line
		for (unsigned x = 0; x < WIDTH; ++x)
		{
			cells[line][x].isFilled = false;
			cells[line][x].drawable.setFillColor(Cell::EMPTY_COLOR);
		}
		// Push down the lines above by one for each filled line
		for (int y = line - 1; y >= 0; --y)
		{
			for (unsigned x = 0; x < WIDTH; ++x)
			{
				if (cells[y][x].isFilled)
				{
					cells[y + 1][x].isFilled = true;
					cells[y + 1][x].drawable.setFillColor(cells[y][x].drawable.getFillColor());
					cells[y][x].isFilled = false;
					cells[y][x].drawable.setFillColor(Cell::EMPTY_COLOR);
				}
			}
		}
	}
}

void Grid::fillCell(sf::Vector2u position, const sf::Color& color)
{
	if (position.x < WIDTH && position.y < HEIGHT)
	{
		cells[position.y][position.x].isFilled = true;
		cells[position.y][position.x].drawable.setFillColor(color);
	}
	else
		std::cerr << "Error: Attempted to fill a cell outside the grid bounds." << std::endl;
}

bool Grid::isTetrominoAtValidPosition(const Tetromino& tetromino)
{
	const auto& shape = tetromino.getShape();
	const auto& pos = tetromino.position;

	for (unsigned y = 0; y < 4; ++y)
	{
		for (unsigned x = 0; x < 4; ++x)
		{
			if (shape[y][x])
			{
				int gridX = static_cast<int>(pos.x) + x;
				int gridY = static_cast<int>(pos.y) + y;

				// Check if the position is outside the grid bounds
				if (gridX < 0 || gridX >= WIDTH || gridY < 0 || gridY >= HEIGHT)
					return false;

				// Check if the cell is already filled
				if (cells[gridY][gridX].isFilled)
					return false;
			}
		}
	}
	// If all checks pass, the position is valid
	return true;
}
