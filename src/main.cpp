#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

sf::Text getText(sf::Font &font);
sf::Font getFont();
std::map<std::string, sf::Texture> textureFiles;
std::map<std::string, sf::Font> fontFiles;
std::map<std::string, sf::SoundBuffer> audioFiles;

void loadResources();
void loadTexture(std::string filePath);
void loadSoundBuffer(std::string filePath);
void loadFont(std::string filePath);

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Jam");
  sf::Font font = getFont();
  sf::Text text = getText(font);
  while (window.isOpen()) {
    // Rendering
    window.clear(sf::Color::Black);

	window.draw(getText(font));

    window.display();

    // Event loop
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;
      }
    }
  }

  return 0;
}

sf::Font getFont() {
	sf::Font font;
	if (!font.loadFromFile("assets/gamefont.ttf"))
	{

	}
	return font;
}

sf::Text getText(sf::Font &font) {
	sf::Text text;
	text.setFont(font);
	text.setString("Hello world");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	return text;
}

void loadResources() {

}

void loadTexture(std::string filePath)
{
	// Textures
	sf::Texture texture = sf::Texture();
	if (!texture.loadFromFile(filePath))
	{
		// error
	}
	textureFiles[filePath];
}