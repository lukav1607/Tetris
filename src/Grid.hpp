// ================================================================================================
// File: Grid.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Defines the Grid class, which is responsible for creating and managing a grid of cells.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Cell.hpp"

class Grid : public sf::Drawable
{
public:
	static constexpr unsigned WIDTH = 10u;
	static constexpr unsigned HEIGHT = 20u;
	static constexpr sf::Vector2u OFFSET = { 50u, 50u };
	static constexpr sf::Color OUTLINE_COLOR = sf::Color(243, 214, 67);

	Grid();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void fillCell(sf::Vector2u position, const sf::Color& color);

	// Check for filled lines and return their indices
	std::vector<unsigned> getFilledLines();
	// Clear the filled lines and push down the lines above by number of filled lines
	void clearFilledLinesAndPushDown(const std::vector<unsigned>& filledLines);

	bool isCellFilled(sf::Vector2u position) const;

private:
	std::vector<std::vector<Cell>> cells;
	sf::RectangleShape outline;
};