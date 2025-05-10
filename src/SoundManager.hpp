// ================================================================================================
// File: SoundManager.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 10, 2025
// Description: Defines the SoundManager class, which is responsible for managing sound effects in the game.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <unordered_map>
#include <SFML/Audio.hpp>

class SoundManager
{
public:
	enum class SoundID
	{
		GAME_START,
		COLLISION,
		LINE_CLEAR,
		LEVEL_UP,
		PAUSE,
		GAME_OVER
	};

	void loadSounds();

	// Removes any sounds that are no longer playing every `interval` seconds 
	void cleanupSounds(float fixedTimeStep, float interval);

	// Play a sound at specified volume with a random pitch variation offset from `basePitch`
	// Note: pitchVariancePercentage 0.15f == +/- 15% variation
	void playSound(SoundID soundID, float pitchVariancePercentage = 0.f, float basePitch = 1.f, float volumeMultiplier = 1.f);

	// Play a sound at specified volume with specified pitch.
	void playSoundAtPitch(SoundID soundID, float pitch, float volumeMultiplier = 1.f);

	float volume = 100.f;

private:
	std::unordered_map<SoundID, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
	std::vector<std::shared_ptr<sf::Sound>> activeSounds;
};