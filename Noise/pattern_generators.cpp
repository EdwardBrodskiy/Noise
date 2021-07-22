#include "pattern_generators.h"
#include "global_values.h"
#include "canvas_tools.h"

void worley_noise(sf::Uint8* pixels, int number_of_points) {
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

void lines(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent) {

	double angle = 0.3;

	double ca = 200;
	double cb = 1100;
	double* cross = lines_cross(angle, ca, -angle, cb);

	draw_line(pixels, angle, 200, accent);
	draw_line(pixels, -angle, 1100, accent);
	draw_vertical_line(pixels, cross[0], accent);

	for (double i = -angle * (double)(width); i < (angle + 1) * (double)(width); i += 90) {
		draw_line(pixels, angle, i, primary);
		draw_line(pixels, -angle, i, primary);
	}
}

void graph_lines(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent) {

	double angle = 0.3;

	double ca = 200;
	double cb = 1100;
	double* cross = lines_cross(angle, ca, -angle, cb);

	draw_line(pixels, angle, 200, accent);
	draw_line(pixels, -angle, 1100, accent);
	draw_vertical_line(pixels, cross[0], accent);

	for (double i = -angle * (double)(width); i < (angle + 1) * (double)(width); i += 5) {
		primary[3] = 255 * pow((1 - abs(sin(i / 3.14159 / 15)) / 5), 4);
		draw_line(pixels, angle, i, primary);
		draw_line(pixels, -angle, i, primary);
	}
}

void sin_storm(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent) {

	std::srand(std::time(nullptr));
	for (int wave = 0; wave < 10; wave++) {
		double freq_shift = (double)(std::rand() % 100) / 75;
		double faze_shift = (double)(std::rand() % 100) / (3.14159 * 100);
		int prev_y = height / 2;
		sf::Uint8* color = primary;
		if (wave == 8) {
			color = accent;
		}
		for (int x = 0; x < width; x++) {
			int y = (height + sin((((double)x * 3 / width + faze_shift) * 3.14159) * (1 + freq_shift)) * pow((double)x * (wave + 1) / width * 3, 2)) / 2;
			draw_line(pixels, x - 1, prev_y, x, y, color);
			prev_y = y;
		}
	}

}

void sin_wave(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent) {

	int waves = 30;
	for (int wave = -waves; wave < waves; wave++) {
		int prev_y = height / 2;
		sf::Uint8* color = primary;
		if (wave == -waves / 2) {
			color = accent;
		}
		for (int x = 0; x < width; x++) {
			double y_shift = (double)wave / waves * height;
			double x_shift = y_shift * height / width * 1.3;
			int y = .6 * x + sin(((((double)x + x_shift) * 10 / width) * 3.14159)) * 10 * abs(wave) + y_shift;
			draw_line(pixels, x - 1, prev_y, x, y, color);
			prev_y = y;
		}
	}

}