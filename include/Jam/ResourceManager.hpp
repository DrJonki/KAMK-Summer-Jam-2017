#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace jam {
  class ResourceManager {
    public:
      sf::Texture& GetTexture(const std::string& filePath);
      sf::SoundBuffer& GetSoundBuffer(const std::string& filePath);
      sf::Font& GetFont(const std::string& filePath);
    private:
      std::map<std::string, sf::Texture> textureFiles;
      std::map<std::string, sf::SoundBuffer> audioFiles;
      std::map<std::string, sf::Font> fontFiles;
  };
}