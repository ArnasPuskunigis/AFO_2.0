#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AudioManager {
public:
    void load(const std::string& name, const std::string& filename);
    void play(const std::string& name, int volume = 100, bool loop = false);

private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::map<std::string, sf::Sound> sounds;
};