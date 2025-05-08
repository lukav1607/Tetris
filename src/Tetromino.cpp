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
	position(START_POSITION)
{
	sf::RectangleShape drawable(sf::Vector2f(Cell::SIZE - 1, Cell::SIZE - 1));
	drawable.setFillColor(COLORS.at(static_cast<int>(type)));
	drawables.fill(drawable);
}

Tetromino& Tetromino::operator=(const Tetromino& other)
{
	if (this != &other)
	{
		type = other.type;
		shape = other.shape;
		position = other.position;
		drawables = other.drawables;
	}
	return *this;
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

void Tetromino::rotate()
{

}