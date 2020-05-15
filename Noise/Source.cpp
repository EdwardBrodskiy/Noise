#include <iostream>
#include <SFML\Graphics.hpp>

int height = 1080;
int width = 1920;

int main() {

	sf::RenderWindow window(sf::VideoMode(width, height), "Noise", sf::Style::Fullscreen);

	// Object used to put the image on screen
	sf::Texture texture;
	sf::Sprite sprite;
	texture.create(width, height);
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);

	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	while (window.isOpen()) {

		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			switch (evnt.type) {
			case sf::Event::Closed:
				window.close();
			}

		}

		window.clear(sf::Color::Black);

		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

		pixels[(mouse_pos.x + mouse_pos.y * width) * 4] = 255;
		pixels[(mouse_pos.x + mouse_pos.y * width) * 4 + 3] = 255;

		texture.update(pixels);

		window.draw(sprite);



		window.display();
	}
	

}