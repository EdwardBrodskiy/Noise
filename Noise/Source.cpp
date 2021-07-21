#include <iostream>
#include <algorithm>
#include <SFML\Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <set>  

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

void graph_lines(sf::Uint8* pixels);

void sin_storm(sf::Uint8* pixels);

void haze(sf::Uint8* pixels);

void sin_wave(sf::Uint8* pixels);


int main() {

	int what = 3;
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
				worley_noise(pixels);
				break;
			case 1:
				lines(pixels);
				break;
			case 2:
				graph_lines(pixels);
				break;
			case 3:
				sin_storm(pixels);
				break;
			case 4:
				sin_wave(pixels);
				break;	}
			haze(pixels);
			once = false;
			image.create(width, height, pixels);

			std::set<unsigned long int> colors;

			for (int i = 0; i < width * height; i++) {
				unsigned long int color_hash = 0;
				for (int shift = 0; shift < 4; shift++) {
					color_hash += (unsigned long int)pixels[i * 4 + shift] * pow(256, shift);
				}
				auto search = colors.find(color_hash);
				if (search == colors.end() ){ // if not found
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
				int b = c % 256 ;
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

int coord_to_index(int x, int y) {
	return (x + y * width) * 4;
}

bool is_in(int x, int y) {
	return x >= 0 && y >= 0 && x < width && y < height;
}

bool is_empty(sf::Uint8* pixels, int x, int y) {
	int index = coord_to_index(x, y);
	for (int i = 3; i < 4; i++) {
		if (pixels[index + i] > 0) {
			return false;
		}
	}
	return true;
}

double distance(int x, int y, int xn, int yn) {
	return pow(pow(xn - x, 2) + pow(yn - y, 2), .5);
}

double* lines_cross(double ma, double ca, double mb, double cb) {
	double x = (ca - cb) / (mb - ma);
	double y = ma * x + ca;
	double coords[] = { x, y };
	return coords;
}

void draw_line(sf::Uint8* pixels, double m, double c, sf::Uint8* color,int start= 0, int end= width - 1) {
	
	if (-1 < m && m < 1) {
		int direction = (end > start) * 2 - 1;
		for (int x = start; x != end; x += direction) {
			int y = m * x + c;
			if (is_in(x, y)) {
				int index = coord_to_index(x, y);
				for (int i = 0; i < 4; i++) {
					pixels[index + i] = color[i];
				}
			}
		}
	}
	else {
		int y_start = m * start + c;
		int y_end = m * end + c;
		int direction = (y_end > y_start) * 2 - 1;
		for (int y = y_start; y != y_end; y += direction) {
			int x = (double)(y - c) / m;
			if (is_in(x, y) && start <= x && x < end) {
				int index = coord_to_index(x, y);
				for (int i = 0; i < 4; i++) {
					sf::Uint8 new_color = color[i];
					if (new_color < pixels[index + i]) {
						new_color = pixels[index + i];
					}
					pixels[index + i] = new_color;
				}
			}
		}
	}
	
}

void draw_line(sf::Uint8* pixels, int x, int y, int nx, int ny, sf::Uint8* color) {
	double m = (double)((long int)y - ny) / (double)((long int)x - nx);
	double c = (double)y - m * x;
	draw_line(pixels, m, c, color,x, nx);
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

	for (double i = -angle * (double)(width); i < (angle + 1) * (double)(width); i += 90) {
		draw_line(pixels, angle, i, blue);
		draw_line(pixels, -angle, i, blue);
	}
}

void graph_lines(sf::Uint8* pixels) {
	sf::Uint8 blue[] = { 78, 94, 109, 255 };
	sf::Uint8 accent[] = { 225, 20, 20, 255 };
	double angle = 0.3;

	double ca = 200;
	double cb = 1100;
	double* cross = lines_cross(angle, ca, -angle, cb);

	draw_line(pixels, angle, 200, accent);
	draw_line(pixels, -angle, 1100, accent);
	draw_vertical_line(pixels, cross[0], accent);

	for (double i = -angle * (double)(width); i < (angle + 1) * (double)(width); i += 5) {
		blue[3] = 255 * pow((1 - abs( sin(i / 3.14159 / 15)) / 5), 4);
		draw_line(pixels, angle, i, blue);
		draw_line(pixels, -angle, i, blue);
	}
}

void sin_storm(sf::Uint8* pixels) {
	sf::Uint8 blue[] = { 78, 94, 109, 255 };
	sf::Uint8 accent[] = { 225, 20, 20, 255 };
	std::srand(std::time(nullptr));
	for (int wave = 0; wave < 10; wave++) {
		double freq_shift = (double)(std::rand() % 100) / 75;
		double faze_shift = (double)(std::rand() % 100) / (3.14159 * 100);
		int prev_y = height / 2;
		sf::Uint8* color = blue;
		if (wave == 8) {
			color = accent;
		}
		for (int x = 0; x < width; x++) {
			int y =( height + sin((((double)x  * 3  / width + faze_shift)* 3.14159) * (1 + freq_shift)) * pow((double)x * (wave + 1) / width * 3, 2) ) / 2;
			draw_line(pixels, x - 1, prev_y, x, y, color);
			prev_y = y;
		}
	}
	
}

void haze(sf::Uint8* pixels) {
	auto* buffer = new double[width * height * 4];

	auto* light_counter = new double[width * height];


	for (int i = 0; i < width * height * 4; i++) {
		buffer[i] = 0;
	}

	for (int i = 0; i < width * height; i++) {
		light_counter[i] = 0;
	}
	int radius = 100;
	double intensity = 8;
	double dispersion = 6;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (!is_empty(pixels, x, y)) {
				for (int xd = x - radius; xd < x + radius; xd++) {
					for (int yd = y - radius; yd < y + radius; yd++) {
						if (is_in(xd,yd)) {
							int origin = coord_to_index(x, y);
							int target = coord_to_index(xd, yd);

							double dist = distance(x, y, xd, yd);
							double drop_off_factor = intensity * 0.01 * (pow(3, -0.03 / dispersion * pow(dist, 2)) + pow(6, -pow(dist, 2)));

							light_counter[target / 4] += drop_off_factor;

							for (int i = 0; i < 3; i++) {
								buffer[target + i] += (double)pixels[origin + i] * drop_off_factor;
							}
							buffer[target + 3] += (int)pixels[origin + 3] * pow(drop_off_factor * 1.5, .5) ;

							
						}
						
					}
				}
				
			}
		}
	}

	for (int i = 0; i < width * height; i++) {
		for (int shift = 0; shift < 3; shift++) {
			double new_color = std::min((int)(buffer[i * 4 + shift]) + (int)pixels[i * 4 + shift], 255);
			double color_space = (double)(buffer[i * 4 + shift]) / light_counter[i];
			pixels[i * 4 + shift] = new_color * (256 - color_space) / 256 + color_space;
		}
		pixels[i * 4 + 3] = std::min((int)(buffer[i * 4 + 3] / 4 ), 255)/ 8 + 32 * 7;
	}


	delete[] buffer;
	delete[] light_counter;
}

void sin_wave(sf::Uint8* pixels) {
	sf::Uint8 blue[] = { 172, 197, 218, 255 }; //{ 119, 147, 175, 255 };
	sf::Uint8 accent[] = { 255, 26, 26, 255 };
	int waves = 30;
	for (int wave = -waves; wave < waves; wave++) {
		int prev_y = height / 2;
		sf::Uint8* color = blue;
		if (wave == -waves / 2) {
			color = accent;
		}
		for (int x = 0; x < width; x++) {
			double y_shift = (double)wave / waves * height;
			double x_shift = y_shift * height / width * 1.3;
			int y = .6 * x + sin(((((double)x + x_shift) * 10 / width) * 3.14159)) * 10 * abs(wave) + y_shift ;
			draw_line(pixels, x - 1, prev_y, x, y, color);
			prev_y = y;
		}
	}

}