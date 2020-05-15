#include <iostream>
#include <ctime>
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

	sf::Vector2i points[20];

	std::srand(std::time(nullptr));
	for (int i = 1; i < 20; i++) {
		points[i].x = std::rand() % width;
		points[i].y = std::rand() % height;
	}

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

		points[0] = sf::Mouse::getPosition(window);

		sf::Uint8* pixels = new sf::Uint8[width * height * 4];

		for (int i = 0; i < 20; i++) {
			unsigned int index = (points[i].x + points[i].y * width) * 4;
			pixels[index] = 255;
			pixels[index + 3] = 255;
		}

		texture.update(pixels);

		window.draw(sprite);



		window.display();
	}
	

}

