// ================================================================================================
// File: HUD.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 9, 2025
// Description: Defines the HUD class, which is responsible for displaying the game information
//              such as score, level, and lines cleared on the screen.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

class HUD : public sf::Drawable
{
public:
	HUD(const sf::Font& font);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	inline void updateScore(int score) { this->score.setString("SCORE: " + std::to_string(score)); }
	inline void updateLevel(int level) { this->level.setString("LEVEL: " + std::to_string(level)); }
	inline void updateLinesCleared(int linesCleared) { this->linesCleared.setString("LINES: " + std::to_string(linesCleared)); }

private:
	sf::Text score;
	sf::Text level;
	sf::Text linesCleared;
	sf::Text nextTetromino;

	sf::Color textColor;
};