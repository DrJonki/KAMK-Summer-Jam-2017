#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceManager {
	public:
		void Load();
		void GetTextures();
		void GetSoundBuffers();
		void GetFonts();
	private:
		void LoadTextures();
		void LoadSoundBuffers();
		void LoadFonts();
		std::map<std::string, sf::Texture> textureFiles;
		std::map<std::string, sf::Font> fontFiles;
		std::map<std::string, sf::SoundBuffer> audioFiles;
};