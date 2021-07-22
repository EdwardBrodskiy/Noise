#include "after_effects.h"
#include "global_values.h"
#include "canvas_tools.h"

void haze(sf::Uint8* pixels, int radius, double intensity, double dispersion) {
	auto* buffer = new double[width * height * 4];

	auto* light_counter = new double[width * height];


	for (int i = 0; i < width * height * 4; i++) {
		buffer[i] = 0;
	}

	for (int i = 0; i < width * height; i++) {
		light_counter[i] = 0;
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (!is_empty(pixels, x, y)) {
				for (int xd = x - radius; xd < x + radius; xd++) {
					for (int yd = y - radius; yd < y + radius; yd++) {
						if (is_in(xd, yd)) {
							int origin = coord_to_index(x, y);
							int target = coord_to_index(xd, yd);

							double dist = distance(x, y, xd, yd);
							double drop_off_factor = intensity * 0.05 * (0.02 * pow(3, -0.03 / dispersion * pow(dist, 2)) + pow(6, -pow(dist, 2)));

							light_counter[target / 4] += drop_off_factor;

							for (int i = 0; i < 3; i++) {
								buffer[target + i] += (double)pixels[origin + i] * drop_off_factor;
							}
							buffer[target + 3] += (int)pixels[origin + 3] * pow(drop_off_factor * 1.5, .5);


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
		pixels[i * 4 + 3] = std::min((int)(buffer[i * 4 + 3] / 4), 255) / 8 + 32 * 7;
	}


	delete[] buffer;
	delete[] light_counter;
}