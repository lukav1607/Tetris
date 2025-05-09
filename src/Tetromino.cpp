// ================================================================================================
// File: Tetromino.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Tetromino.hpp"
#include "Grid.hpp"

Tetromino::Tetromino(Tetromino::Type type) :
	type(type),
	shape(SHAPES.at(static_cast<int>(type))),
	color(COLORS.at(static_cast<int>(type)))
{
	updateStartPosition();
	sf::RectangleShape drawable(sf::Vector2f(Cell::SIZE - 1, Cell::SIZE - 1));
	drawable.setFillColor(color);
	drawables.fill(drawable);
}

Tetromino& Tetromino::operator=(const Tetromino& other)
{
	if (this != &other)
	{
		type = other.type;
		shape = other.shape;
		color = other.color;
		position = other.position;
		drawables = other.drawables;
	}
	return *this;
}

void Tetromino::updateStartPosition()
{
	if (this->type == Type::I)
		position = START_POSITION + sf::Vector2f(0.f, -1.f);
	else if (this->type == Type::O)
		position = START_POSITION + sf::Vector2f(1.f, 0.f);
	else
		position = START_POSITION;
}

void Tetromino::updateDrawPosition()
{
	unsigned i = 0;
	for (unsigned y = 0; y < 4; ++y)
	{
		for (unsigned x = 0; x < 4; ++x)
		{
			if (shape[y][x])
			{
				drawables.at(i).setPosition(sf::Vector2f((position.x + x) * Cell::SIZE + Grid::OFFSET.x, (position.y + y) * Cell::SIZE + Grid::OFFSET.y));
				++i;
			}
		}
	}
}

void Tetromino::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& drawable : drawables)
		target.draw(drawable);
}

bool Tetromino::tryMove(sf::Vector2i offset, const Grid& grid)
{
	position += sf::Vector2f(offset);
	if (isAtValidPosition(grid))
		return true;
	position -= sf::Vector2f(offset);
	return false;
}

bool Tetromino::tryRotateCW(const Grid& grid)
{
	Shape original = shape;
	sf::Vector2f originalPosition = position;

	rotateCW();

	if (isAtValidPosition(grid)) 
		return true;

	// Try small adjustments to the position if the rotation fails
	const std::vector<sf::Vector2f> offsets =
	{
		{ -1, 0 }, { 1, 0 }, { -2, 0 }, { 2, 0 }//, { 0, 1 }
	};

	for (const auto& offset : offsets)
	{
		position += offset;
		if (isAtValidPosition(grid))
			return true;
		position = originalPosition;
	}

	// If all attempts fail, revert to the original shape and position
	shape = original;
	position = originalPosition;
	return false;
}

void Tetromino::rotateCW()
{
	// O tetromino does not change shape when rotated
	if (type == Type::O)
		return;

	Shape rotatedShape{};

	// I tetromino rotates around center, but due to its straight shape we handle it specially
	if (type == Type::I)
	{
		// 2-state toggle rotation (simplified for classic 1D I shape)
		bool horizontal = shape[1][0] == 1;
		if (horizontal)
		{
			// Rotate to vertical
			for (int i = 0; i < 4; ++i)
				rotatedShape[i][1] = 1;
		}
		else
		{
			// Rotate to horizontal
			for (int i = 0; i < 4; ++i)
				rotatedShape[1][i] = 1;
		}
	}
	// For all other tetrominoes
	else
	{
		// Rotate the 3x3 submatrix inside the 4x4 matrix
		for (unsigned y = 0; y < 3; ++y)
		{
			for (unsigned x = 0; x < 3; ++x)
			{
				rotatedShape[x][size_t(2 - y)] = shape[y][x];
			}
		}
	}

	shape = rotatedShape;
}

void Tetromino::rotateCCW()
{
	Shape rotatedShape{};

	for (unsigned y = 0; y < 4; ++y)
	{
		for (unsigned x = 0; x < 4; ++x)
		{
			rotatedShape[size_t(3 - x)][y] = shape[y][x];
		}
	}
	shape = rotatedShape;
}

bool Tetromino::isAtValidPosition(const Grid& grid) const
{
	for (unsigned y = 0; y < 4; ++y)
	{
		for (unsigned x = 0; x < 4; ++x)
		{
			if (shape[y][x])
			{
				int gridX = static_cast<int>(position.x) + x;
				int gridY = static_cast<int>(position.y) + y;

				// Check if the position is outside the grid bounds
				if (gridX < 0 || gridX >= Grid::WIDTH || gridY < 0 || gridY >= Grid::HEIGHT)
					return false;

				// Check if the cell is already filled
				if (grid.isCellFilled(sf::Vector2u(gridX, gridY)))
					return false;
			}
		}
	}
	// If all checks pass, the position is valid
	return true;
}
