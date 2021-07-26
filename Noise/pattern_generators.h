#pragma once
#include <SFML\Graphics.hpp>


void worley_noise(sf::Uint8* pixels, int number_of_points);

void lines(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void graph_lines(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void sin_storm(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void sin_wave(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void dots(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void fractal_tree(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void aux_fractal_tree(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent, sf::Vector2f start, sf::Vector2f branch, float size_change, float angle_change);
