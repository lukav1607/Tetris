// ================================================================================================
// File: Cell.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Defines the Cell class, which represents a single cell in the grid.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

struct Cell : public sf::Drawable
{
	static constexpr unsigned SIZE = 50u;

	Cell() :
		shape(sf::Vector2f(SIZE, SIZE)),
		isFilled(false)
	{
		shape.setFillColor(sf::Color::Black);
		shape.setOutlineThickness(1.f);
		shape.setOutlineColor(sf::Color::White);
	}
	inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(shape, states); }

	sf::RectangleShape shape;
	bool isFilled;
};