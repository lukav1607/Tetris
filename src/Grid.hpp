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

	Grid();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<std::vector<Cell>> cells;
};