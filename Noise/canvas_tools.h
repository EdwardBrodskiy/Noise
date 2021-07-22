#pragma once
#include <SFML\Graphics.hpp>
#include "global_values.h"

double* calculate_distances(int x, int y, sf::Vector2i* points, int num_points);

int coord_to_index(int x, int y);

bool is_in(int x, int y);

bool is_in(int index);

bool is_empty(sf::Uint8* pixels, int x, int y);

double distance(int x, int y, int xn, int yn);

double* lines_cross(double ma, double ca, double mb, double cb);

bool approx_equals(double x, double y, double x1, double y1, double error);

void draw_line(sf::Uint8* pixels, double m, double c, sf::Uint8* color, int start = 0, int end = width - 1);

void draw_line(sf::Uint8* pixels, int x, int y, int nx, int ny, sf::Uint8* color);

void draw_vertical_line(sf::Uint8* pixels, int x, sf::Uint8* color);

void draw_circle(sf::Uint8* pixels, int x, int y, int r, sf::Uint8* color);
