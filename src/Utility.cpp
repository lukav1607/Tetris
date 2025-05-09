// ================================================================================================
// File: Utility.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 8, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <map>
#include "Utility.hpp"

bool Utility::isKeyReleased(sf::Keyboard::Key key)
{
	static std::map<sf::Keyboard::Key, bool> keyStates;
	if (keyStates.find(key) == keyStates.end())
		keyStates[key] = false;
	bool isPressedNow = sf::Keyboard::isKeyPressed(key);
	bool wasPressedLastFrame = keyStates[key];
	keyStates[key] = isPressedNow;
	return !isPressedNow && wasPressedLastFrame;
}

sf::Color Utility::lerpColor(const sf::Color& a, const sf::Color& b, float t)
{
	return sf::Color(
		static_cast<std::uint8_t>(a.r + (b.r - a.r) * t),
		static_cast<std::uint8_t>(a.g + (b.g - a.g) * t),
		static_cast<std::uint8_t>(a.b + (b.b - a.b) * t)
	);
}