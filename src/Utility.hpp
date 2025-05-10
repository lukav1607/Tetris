// ================================================================================================
// File: Utility.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// Description: Defines the Utility namespace, which contains useful utility functions.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>

namespace Utility
{
	bool isKeyReleased(sf::Keyboard::Key key);
	sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t);
	// Generates a random pitch value based on a given variation percentage.
	// Example use: variationPercent 0.15f == 15% variation
	float randomPitch(float variationPercent, float basePitch = 1.f);
}