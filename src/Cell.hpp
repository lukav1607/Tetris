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
		drawable(sf::Vector2f(SIZE, SIZE)),
		isFilled(false)
	{
		drawable.setFillColor(sf::Color::Black);
		drawable.setOutlineThickness(1.f);
		drawable.setOutlineColor(sf::Color::White);
	}
	inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(drawable, states); }

	sf::RectangleShape drawable;
	bool isFilled;
};