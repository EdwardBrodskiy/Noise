#include <iostream>
#include <ctime>
#include <SFML\Graphics.hpp>
#include <cmath>
#include <vector>

int number_of_points = 80;

int height = 1080;
int width = 1920;

unsigned int diagonal = (unsigned int)std::sqrt(std::pow((width), 2) + std::pow((height), 2));

double* calculate_distances(int x, int y, sf::Vector2i* points, int num_points) {
	double* distances = new double[num_points];
	for (int i = 0; i < num_points; i++) {
		distances[i] = std::sqrt(std::pow((x - points[i].x), 2) + std::pow((y - points[i].y), 2));
	}

	return distances;
}



int main() {

	unsigned int x = 6954;
	uint8_t m = (uint8_t)x;

	sf::RenderWindow window(sf::VideoMode(width, height), "Noise", sf::Style::Fullscreen);

	// Object used to put the image on screen
	sf::Texture texture;
	sf::Sprite sprite;
	texture.create(width, height);
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);

	sf::Vector2i* points = new sf::Vector2i[number_of_points];

	

	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	while (window.isOpen()) {

		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			switch (evnt.type) {
			case sf::Event::Closed:
				window.close();
			}

		}

		std::srand(std::time(nullptr));
		for (int i = 0; i < number_of_points; i++) {
			points[i].x = std::rand() % width;
			points[i].y = std::rand() % height;
		}
		
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				unsigned int index = (x + y * width) * 4;
				double* distances = calculate_distances(x, y, points, number_of_points);
				std::sort(distances, distances + number_of_points);

				for (int i = 1; i < 3; i++) {
					int distance = (int)distances[i];
					if (distance < 256) {
						pixels[index + i] = 255 - distance;
					}
					else {
						pixels[index + i] = 0;
					}
				}
				
				pixels[index + 3] = 255;// (int)distances[0];
				delete distances;
			}
		}

		window.clear(sf::Color::Black);

		texture.update(pixels);

		window.draw(sprite);



		window.display();
	}
	

}

