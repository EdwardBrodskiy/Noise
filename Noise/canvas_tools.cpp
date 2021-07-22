#include "canvas_tools.h"
#include "global_values.h"
#include <cmath>


double* calculate_distances(int x, int y, sf::Vector2i* points, int num_points) {
	double* distances = new double[num_points];
	for (int i = 0; i < num_points; i++) {
		distances[i] = std::sqrt(std::pow((x - points[i].x), 2) + std::pow((y - points[i].y), 2));
	}

	return distances;
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

void draw_line(sf::Uint8* pixels, double m, double c, sf::Uint8* color, int start, int end) {

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
	draw_line(pixels, m, c, color, x, nx);
}

void draw_vertical_line(sf::Uint8* pixels, int x, sf::Uint8* color) {

	for (int y = 0; y < height; y++) {
		int index = coord_to_index(x, y);
		for (int i = 0; i < 4; i++) {
			pixels[index + i] = color[i];
		}
	}


}