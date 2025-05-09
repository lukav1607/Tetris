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
	static constexpr sf::Color EMPTY_COLOR = sf::Color(25, 26, 27);

	Cell() :
		drawable(sf::Vector2f(SIZE, SIZE)),
		color(EMPTY_COLOR),
		isFilled(false)
	{
		drawable.setFillColor(color);
		drawable.setOutlineThickness(1.f);
		drawable.setOutlineColor(sf::Color::Black);
	}
	inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(drawable, states); }

	sf::RectangleShape drawable;
	sf::Color color;
	bool isFilled;
};