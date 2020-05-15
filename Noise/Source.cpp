#include <iostream>
#include <SFML\Graphics.hpp>

int height = 1080;
int width = 1920;

int main() {

	sf::RenderWindow window(sf::VideoMode(width, height), "MandelBrotSet", sf::Style::Fullscreen);

	// Object used to put the image on screen
	sf::Texture image;
	sf::Sprite sprite;
	image.create(width, height);
	sprite.setTexture(image);
	sprite.setPosition(0, 0);

	while (true) {
		window.draw(sprite);
		window.display();
	}
	

}