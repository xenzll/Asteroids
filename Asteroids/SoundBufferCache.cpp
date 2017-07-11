#include "SoundBufferCache.h"

#include <vector>
#include <exception>

namespace Asteroids
{

std::map<std::string, sf::SoundBuffer> SoundBufferCache::stringToBuffer;

void SoundBufferCache::LoadSoundBuffers()
{
	static const std::vector<std::string> AudioFiles
	{
		"thrust",
		"fire",
		"bangSmall",
		"bangMedium",
		"bangLarge"
	};

	static const std::string FileExtension	= ".wav";
	static const std::string AudioDirectory = "Resources/Audio/";

	for (size_t i = 0; i < AudioFiles.size(); ++i)
	{
		sf::SoundBuffer buffer;

		bool successfulLoad = buffer.loadFromFile(AudioDirectory + AudioFiles[i] + FileExtension);

		if (!successfulLoad)
		{
			throw std::runtime_error("Failed to load audio buffer: " + AudioFiles[i]);
		}
		else
		{
			stringToBuffer[AudioFiles[i]] = std::move(buffer);
		}
	}
}

SoundBufferCache& SoundBufferCache::GetInstance()
{
	static SoundBufferCache instance;
	return instance;
}

const sf::SoundBuffer& SoundBufferCache::GetSoundBuffer(const std::string& fileName)
{
	auto result = stringToBuffer.find(fileName);

	return result->second;
}

} // namespace Asteroids