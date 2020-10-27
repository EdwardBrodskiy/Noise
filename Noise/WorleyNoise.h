#pragma once
#include <SFML\Graphics.hpp>

class WorleyNoise
{
private:
	int number_of_points;
	int width, height;

	sf::Uint8* pixels;

public:
	WorleyNoise(int width_in, int height_in, int number_of_points_in);

	~WorleyNoise();

	bool set_number_of_points(int num);


};

