#include <iostream>
#include <algorithm>
#include <SFML\Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <set>  

#include "global_values.h"
#include "pattern_generators.h"
#include "after_effects.h"

int main() {

	int what = 6;
	sf::Uint8 primary[] = { 172 * 2 / 3, 197 * 2 / 3, 218, 255 };
	sf::Uint8 accent[] = { 255, 26, 26, 255 };

	bool once = true;

	sf::Image image;

	sf::RenderWindow window(sf::VideoMode(width, height), "Noise", sf::Style::Fullscreen);

	// Object used to put the image on screen
	sf::Texture texture;
	sf::Sprite sprite;
	texture.create(width, height);
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);

	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	for (int i = 0; i < width * height * 4; i++) {
		if (i % 4 == 3) {
			pixels[i] = 0;
		}
		else {
			pixels[i] = 0;
		}

	}

	while (window.isOpen()) {

		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			switch (evnt.type) {
			case sf::Event::Closed:
				window.close();
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			once = true;
			for (int i = 0; i < width * height * 4; i++) {
				pixels[i] = 0;
			}
		}


		window.clear(sf::Color::Black);

		texture.update(pixels);

		window.draw(sprite);



		window.display();

		if (once) {
			switch (what) {
			case 0:
				worley_noise(pixels, 30);
				break;
			case 1:
				lines(pixels, primary, accent);
				haze(pixels, 30, 5, 4);
				break;
			case 2:
				graph_lines(pixels, primary, accent);
				haze(pixels, 30, 15, 4);
				break;
			case 3:
				sin_storm(pixels, primary, accent);
				haze(pixels, 30, 15, 4);
				break;
			case 4:
				sin_wave(pixels, primary, accent);
				haze(pixels, 30, 5, 4);
				break;
			case 5:
				dots(pixels, primary, accent);
				haze(pixels, 30, 5, 4);
				break;
			case 6:
				fractal_tree(pixels, primary, accent);
				haze(pixels, 30, 5, 4);
				break;
			}
			once = false;
			image.create(width, height, pixels);

			std::set<unsigned long int> colors;

			for (int i = 0; i < width * height; i++) {
				unsigned long int color_hash = 0;
				for (int shift = 0; shift < 4; shift++) {
					color_hash += (unsigned long int)pixels[i * 4 + shift] * pow(256, shift);
				}
				auto search = colors.find(color_hash);
				if (search == colors.end()) { // if not found
					colors.insert(color_hash);
				}
			}
			std::cout << colors.size() << std::endl;
			for (int i = 0; i < colors.size(); i += 1000) {
				std::cout << ' ';
			}
			std::cout << '|' << std::endl;
			int counter = 0;
			std::for_each(colors.cbegin(), colors.cend(), [&image, &counter](unsigned long int c) {
				int r = c % 256;
				c = c / 256;
				int g = c % 256;
				c = c / 256;
				int b = c % 256;
				c = c / 256;
				int a = c % 256;
				sf::Color color(r, g, b, a);

				image.createMaskFromColor(color, (sf::Uint8)((a - 7 * 32) * 8));

				if (counter % 1000 == 0) {
					std::cout << '#';
				}
				counter++;



				}
			);

			std::cout << '|' << std::endl;

			image.saveToFile(std::to_string(what) + ".png");
			std::cout << "Image saved to " << std::to_string(what) << ".png" << std::endl;

			for (int i = 3; i < width * height * 4; i += 4) {
				pixels[i] = (pixels[i] - 7 * 32) * 8;
			}
		}
	}


}
