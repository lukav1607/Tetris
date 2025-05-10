// ================================================================================================
// File: TitleScreenShapes.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 10, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "TitleScreenShapes.hpp"
#include "Game.hpp"

TitleScreenShapes::TitleScreenShapes()
{
	float size = 100.f;

	shapes.fill(sf::RectangleShape(sf::Vector2f(size, size)));
	for (auto& shape : shapes)
	{
		shape.setOutlineThickness(-1.f);
		shape.setOutlineColor(sf::Color(40, 42, 50, 150));
		shape.setOrigin({ 0.f, size * 1 });
	}

	sf::Vector2f offset = { 0.f, Game::WINDOW_HEIGHT };

	// Neon Blue (J)
	shapes[14].setPosition(offset + sf::Vector2f(0.f, 0.f));
	shapes[14].setFillColor(sf::Color(85, 85, 255));
	shapes[10].setPosition(offset + sf::Vector2f(0.f, -size * 1));
	shapes[10].setFillColor(sf::Color(85, 85, 255));
	shapes[15].setPosition(offset + sf::Vector2f(size * 1, 0.f));
	shapes[15].setFillColor(sf::Color(85, 85, 255));
	shapes[16].setPosition(offset + sf::Vector2f(size * 2, 0.f));
	shapes[16].setFillColor(sf::Color(85, 85, 255));

	// Neon Red (Z)
	shapes[12].setPosition(offset + sf::Vector2f(size * 2, -size * 1));
	shapes[12].setFillColor(sf::Color(255, 80, 80));
	shapes[13].setPosition(offset + sf::Vector2f(size * 3, -size * 1));
	shapes[13].setFillColor(sf::Color(255, 80, 80));
	shapes[17].setPosition(offset + sf::Vector2f(size * 3, 0.f));
	shapes[17].setFillColor(sf::Color(255, 80, 80));
	shapes[18].setPosition(offset + sf::Vector2f(size * 4, 0.f));
	shapes[18].setFillColor(sf::Color(255, 80, 80));

	// Neon Green (S)
	shapes[7].setPosition(offset + sf::Vector2f(0.f, -size * 3));
	shapes[7].setFillColor(sf::Color(0, 255, 128));
	shapes[8].setPosition(offset + sf::Vector2f(0.f, -size * 2));
	shapes[8].setFillColor(sf::Color(0, 255, 128));
	shapes[9].setPosition(offset + sf::Vector2f(size * 1, -size * 2));
	shapes[9].setFillColor(sf::Color(0, 255, 128));
	shapes[11].setPosition(offset + sf::Vector2f(size * 1, -size * 1));
	shapes[11].setFillColor(sf::Color(0, 255, 128));

	// Neon Cyan (I)
	shapes[19].setPosition(offset + sf::Vector2f(size * 5, 0.f));
	shapes[19].setFillColor(sf::Color(0, 255, 255));
	shapes[20].setPosition(offset + sf::Vector2f(size * 6, 0.f));
	shapes[20].setFillColor(sf::Color(0, 255, 255));
	shapes[21].setPosition(offset + sf::Vector2f(size * 7, 0.f));
	shapes[21].setFillColor(sf::Color(0, 255, 255));
	shapes[22].setPosition(offset + sf::Vector2f(size * 8, 0.f));
	shapes[22].setFillColor(sf::Color(0, 255, 255));

	// Soft Neon Yellow (O)
	shapes[6].setPosition(offset + sf::Vector2f(size * 6, -size * 1));
	shapes[6].setFillColor(sf::Color(255, 255, 128));
	shapes[5].setPosition(offset + sf::Vector2f(size * 6, -size * 2));
	shapes[5].setFillColor(sf::Color(255, 255, 128));
	shapes[23].setPosition(offset + sf::Vector2f(size * 7, -size * 1));
	shapes[23].setFillColor(sf::Color(255, 255, 128));
	shapes[2].setPosition(offset + sf::Vector2f(size * 7, -size * 2));
	shapes[2].setFillColor(sf::Color(255, 255, 128));

	// Vibrant Orange (L)
	shapes[4].setPosition(offset + sf::Vector2f(size * 8, -size * 1));
	shapes[4].setFillColor(sf::Color(255, 160, 0));
	shapes[3].setPosition(offset + sf::Vector2f(size * 8, -size * 2));
	shapes[3].setFillColor(sf::Color(255, 160, 0));
	shapes[1].setPosition(offset + sf::Vector2f(size * 8, -size * 3));
	shapes[1].setFillColor(sf::Color(255, 160, 0));
	shapes[0].setPosition(offset + sf::Vector2f(size * 7, -size * 3));
	shapes[0].setFillColor(sf::Color(255, 160, 0));
}

void TitleScreenShapes::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& shape : shapes)
	{
		target.draw(shape, states);
	}
}