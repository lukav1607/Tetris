// ================================================================================================
// File: HUD.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 9, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "HUD.hpp"
#include "Grid.hpp"

HUD::HUD(const sf::Font& font) :
	score(font, "SCORE: 0", 40),
	level(font, "LEVEL: 1", 40),
	linesCleared(font, "LINES: 0", 40),
	nextTetromino(font, "NEXT SHAPE", 30),
	textColor(sf::Color(255, 245, 210))
{
	score.setPosition({ (Grid::WIDTH + 1) * Cell::SIZE + Grid::OFFSET.x, Grid::OFFSET.y + (Grid::HEIGHT - 4) * Cell::SIZE });
	score.setFillColor(textColor);
	score.setOutlineColor(sf::Color::White);
	score.setOutlineThickness(0.5f);

	level.setPosition({ (Grid::WIDTH + 1) * Cell::SIZE + Grid::OFFSET.x, Grid::OFFSET.y + (Grid::HEIGHT - 3) * Cell::SIZE + Cell::SIZE / 2});
	level.setFillColor(textColor);
	level.setOutlineColor(sf::Color::White);
	level.setOutlineThickness(0.5f);

	linesCleared.setPosition({ (Grid::WIDTH + 1) * Cell::SIZE + Grid::OFFSET.x, Grid::OFFSET.y + (Grid::HEIGHT - 2) * Cell::SIZE + Cell::SIZE });
	linesCleared.setFillColor(textColor);
	linesCleared.setOutlineColor(sf::Color::White);
	linesCleared.setOutlineThickness(0.5f);

	nextTetromino.setPosition({ (Grid::WIDTH + 1) * Cell::SIZE + Grid::OFFSET.x + 35.f, Grid::OFFSET.y });
	nextTetromino.setFillColor(textColor);
	nextTetromino.setOutlineColor(sf::Color::White);
	nextTetromino.setOutlineThickness(0.5f);
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(score, states);
	target.draw(level, states);
	target.draw(linesCleared, states);
	target.draw(nextTetromino, states);
}