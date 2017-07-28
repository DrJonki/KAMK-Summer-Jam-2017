#include <Jam/ResourceManager.hpp>
#include <iostream>
#include <cassert>

namespace jam {
	sf::Texture& ResourceManager::GetTexture(const std::string& filePath)
	{
		if (textureFiles.find(filePath) == textureFiles.end()) {
			std::cout << "Loading " + filePath + "..." << std::endl;
			sf::Texture& texture = textureFiles[filePath];
			if (!texture.loadFromFile(filePath))
			{
				assert(false);
			}
			std::cout << "Loading " + filePath + " complete!" << std::endl;
			return texture;
		}
	}

	sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& filePath)
	{
		std::cout << "Loading " + filePath + "..." << std::endl;
		sf::SoundBuffer& soundBuff = audioFiles[filePath];
		if (!soundBuff.loadFromFile(filePath))
		{
			assert(false);
		}
		std::cout << "Loading " + filePath + " complete!" << std::endl;
		return soundBuff;
	}

	sf::Font& ResourceManager::GetFont(const std::string& filePath)
	{
		std::cout << "Loading " + filePath + "..." << std::endl;
		sf::Font font = fontFiles[filePath];
		if (!font.loadFromFile(filePath))
		{
			assert(false);
		}
		std::cout << "Loading " + filePath + " complete!" << std::endl;
		return font;
	}
}
