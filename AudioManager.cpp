#pragma once
#include "AudioManager.h"
#include <iostream>

void AudioManager::load(const std::string& name, const std::string& filename)
{
	sf::SoundBuffer buffer;
	buffer.loadFromFile(filename);
	buffers.insert({ name, buffer });
}

void AudioManager::play(const std::string& name, int volume, bool loop)
{
	if (!buffers.count(name)) {
		std::cout << "Buffer not found, cannot play unloaded audio: " << name << std::endl;
		return;
	}

	if (!sounds.count(name)) {
		std::cout << "New sound: " << name << std::endl;
		sf::Sound sound;
		sounds.insert({ name, sound });
		sounds.at(name).setBuffer(buffers.at(name));
	}
	sounds.at(name).setVolume(volume);
	sounds.at(name).setLoop(loop);
	sounds.at(name).play();
}

