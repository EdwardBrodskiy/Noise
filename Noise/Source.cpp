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

		sf::Uint8* pixel = new sf::Uint8[4]{255, 0, 0, 255};


		texture.update(pixel, 1, 1, mouse_pos.x, mouse_pos.y);

		window.draw(sprite);



		window.display();
	}
	

}