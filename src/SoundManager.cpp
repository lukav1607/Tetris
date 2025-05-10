// ================================================================================================
// File: SoundManager.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 10, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "SoundManager.hpp"
#include "Utility.hpp"

using namespace Utility;

void SoundManager::loadSounds()
{
	auto load = [&](SoundID soundID, const std::string& filename)
		{
			soundBuffers[soundID] = std::make_shared<sf::SoundBuffer>(filename);
		};

	load(SoundID::GAME_START, "assets/sounds/448262__henryrichard__sfx-begin-2.wav");
	load(SoundID::COLLISION, "assets/sounds/321805__lloydevans09__pvc_pipe_hit_1.wav");
	load(SoundID::LINE_CLEAR, "assets/sounds/109662__grunz__success.wav");
	load(SoundID::LEVEL_UP, "assets/sounds/439889__simonbay__lushlife_levelup.wav");
	load(SoundID::PAUSE, "assets/sounds/459344__lilmati__select-granted-03.wav");
	load(SoundID::GAME_OVER, "assets/sounds/382310__mountain_man__game-over-arcade.wav");
}

void SoundManager::playSound(SoundID soundID, float pitchVariancePercentage, float volumeMultiplier)
{
	auto it = soundBuffers.find(soundID);
	if (it != soundBuffers.end())
	{
		auto sound = std::make_shared<sf::Sound>(*it->second);

		if (pitchVariancePercentage < 0.f || pitchVariancePercentage > 1.f) {
			std::cerr << "Warning: Pitch variance percentage must be between 0 and 1! Pitch variance set to default (0%)" << std::endl;
			pitchVariancePercentage = 0.f;
		}
		if (pitchVariancePercentage != 0.f)
			sound->setPitch(Utility::randomPitch(pitchVariancePercentage));

		sound->setVolume(volume * volumeMultiplier);
		sound->play();

		activeSounds.push_back(sound);
	}
	else
	{
		std::cerr << "Error: Sound ID not found!" << std::endl;
	}
}

void SoundManager::cleanupSounds()
{
	activeSounds.erase(
		std::remove_if(activeSounds.begin(), activeSounds.end(),
			[](const std::shared_ptr<sf::Sound>& sound) {
				return sound->getStatus() == sf::Sound::Status::Stopped;
			}),
		activeSounds.end());
}