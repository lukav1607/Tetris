// ================================================================================================
// File: Tetromino.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Defines the Tetromino class, which represents a Tetris piece.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"

class Tetromino : public sf::Drawable
{
public:
	static constexpr sf::Vector2f START_POSITION = { 3.f, 0.f };

	using Shape = std::array<std::array<bool, 4>, 4>;
	enum class Type { I, O, T, S, Z, J, L };

	Tetromino(Type type);
	Tetromino& operator=(const Tetromino& other);

	// Update tetromino start position based on its type
	void updateStartPosition();
	void updateDrawPosition();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Try and move the tetromino by the given offset, returning true if successful
	bool tryMove(sf::Vector2i offset, const Grid& grid);
	// Try and rotate the tetromino clockwise, returning true if successful
	bool tryRotateCW(const Grid& grid);

	bool isAtValidPosition(const Grid& tetromino) const;

	const Shape& getShape() const { return shape; }
	const Type& getType() const { return type; }
	const sf::Color& getColor() const { return color; }

	sf::Vector2f position; // Top-left corner of the tetromino in the grid

private:
	const std::array<Shape, 7> SHAPES =
	{{
		// I
		{{
			{ 0, 0, 0, 0 },
			{ 1, 1, 1, 1 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}},
		// O
		{{
			{ 1, 1, 0, 0 },
			{ 1, 1, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}},
		// T
		{{
			{ 0, 1, 0, 0 },
			{ 1, 1, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}},
		// S
		{{
			{ 0, 1, 1, 0 },
			{ 1, 1, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}},
		// Z
		{{
			{ 1, 1, 0, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}},
		// J
		{{
			{ 1, 1, 1, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}},
		// L
		{{
			{ 0, 0, 1, 0 },
			{ 1, 1, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}}
	}};
	const std::array<sf::Color, 7> COLORS =
	{{
		sf::Color::Cyan,
		sf::Color::Yellow,
		sf::Color::Magenta,
		sf::Color::Green,
		sf::Color::Red,
		sf::Color::Blue,
		sf::Color(255, 165, 0) // Orange
	}};

	// Rotate the tetromino clockwise
	void rotateCW();
	// Rotate the tetromino counter-clockwise
	void rotateCCW();

	std::array<sf::RectangleShape, 4> drawables;
	Shape shape;
	Type type;
	sf::Color color;
};