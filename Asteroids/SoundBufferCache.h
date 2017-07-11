#pragma once
#include <SFML\Audio.hpp>
#include <map>
#include <string>
#include <memory>

/*
Singleton that manages the lifetime of sound buffers.
Rather than having individual objects manage sound
buffers themslves, potentially loading the same audio 
file multiple times into memory, the sound buffer cache
seeks to alleviate this issue by maintaining only
one instance of any given sound buffer.
*/

namespace Asteroids
{

class SoundBufferCache
{

public:

	static void LoadSoundBuffers();

	static SoundBufferCache& GetInstance();
	~SoundBufferCache() = default;

	static const sf::SoundBuffer& GetSoundBuffer(const std::string& bufferName);
	
private:

	SoundBufferCache() = default;

	static std::map<std::string, sf::SoundBuffer> stringToBuffer;
};

} // namespace Asteroids