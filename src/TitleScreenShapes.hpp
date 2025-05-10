// ================================================================================================
// File: TitleScreenShapes.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 10, 2025
// Description: Defines the TitleScreenShapes class, which is responsible for creating and managing the
//              shapes displayed on the title screen of the game.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

class TitleScreenShapes : public sf::Drawable
{
public:
	TitleScreenShapes();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::array<sf::RectangleShape, 24> shapes;
};