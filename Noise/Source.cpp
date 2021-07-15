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

void worley_noise(sf::Uint8* pixels);

void lines(sf::Uint8* pixels);


int main() {

	int what = 1;
	bool once = true;

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
		if (once) {
			switch (what) {
			case 0:
				worley_noise(pixels);
			case 1:
				lines(pixels);
			}
			once = false;
		}



		window.clear(sf::Color::Black);

		texture.update(pixels);

		window.draw(sprite);



		window.display();
	}


}

int coord_to_index(int x, int y) {
	return (x + y * width) * 4;
}

bool is_in(int x, int y) {
	return x >= 0 && y >= 0 && x < width && y < height;
}

double* lines_cross(double ma, double ca, double mb, double cb) {
	double x = (ca - cb) / (mb - ma);
	double y = ma * x + ca;
	double coords[] = { x, y };
	return coords;
}

void draw_line(sf::Uint8* pixels, double m, double c, sf::Uint8* color) {
	for (int x = 0; x < width; x++) {
		int y = (int)(x * m + c);
		if (is_in(x, y)) {
			int index = coord_to_index(x, y);
			for (int i = 0; i < 4; i++) {
				pixels[index + i] = color[i];
			}
		}
	}
}

void draw_line(sf::Uint8* pixels, int x, int y, int nx, int ny, sf::Uint8* color) {
	double m = (double)((long int)y - ny) / (double)((long int)x - nx);
	double c = (double)y - m * x;
	draw_line(pixels, m, c, color);
}

void draw_vertical_line(sf::Uint8* pixels, int x,  sf::Uint8* color) {

	for (int y = 0; y < height; y++) {
		int index = coord_to_index(x, y);
		for (int i = 0; i < 4; i++) {
			pixels[index + i] = color[i];
		}
	}


}



void worley_noise(sf::Uint8* pixels) {
	sf::Vector2i* points = new sf::Vector2i[number_of_points];

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
}

void lines(sf::Uint8* pixels) {
	sf::Uint8 blue[] = { 78, 94, 109, 255 };
	sf::Uint8 accent[] = { 225, 20, 20, 255 };
	double angle = 0.3;

	double ca = 200;
	double cb = 1100;
	double* cross = lines_cross(angle, ca, -angle, cb);

	draw_line(pixels, angle, 200, accent);
	draw_line(pixels, -angle, 1100, accent);
	draw_vertical_line(pixels, cross[0], accent);

	for (double i = -angle * (double)(width); i < (angle + 1) * (double)(width); i += 30) {
		draw_line(pixels, angle, i, blue);
		draw_line(pixels, -angle, i, blue);
	}
}