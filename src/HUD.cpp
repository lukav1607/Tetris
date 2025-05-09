// ================================================================================================
// File: HUD.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "HUD.hpp"

HUD::HUD(const sf::Font& font) :
	score(font, "Score: 0", 30),
	level(font, "Level: 1", 30),
	linesCleared(font, "Lines: 0", 30),
	nextTetromino(font, "Next Shape:", 30)
{
	score.setPosition({ 10.f, 10.f });
	level.setPosition({ 10.f, 50.f });
	linesCleared.setPosition({ 10.f, 90.f });
	nextTetromino.setPosition({ 10.f, 130.f });
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(score, states);
	target.draw(level, states);
	target.draw(linesCleared, states);
	target.draw(nextTetromino, states);
}