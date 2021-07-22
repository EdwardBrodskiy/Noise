#pragma once
#include <SFML\Graphics.hpp>


void worley_noise(sf::Uint8* pixels, int number_of_points);

void lines(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void graph_lines(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void sin_storm(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);

void sin_wave(sf::Uint8* pixels, sf::Uint8* primary, sf::Uint8* accent);