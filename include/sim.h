#pragma once

#include "SFML/Graphics.hpp"

class Sim
{
    int width;
    int height;

public:
    Sim(int width, int height);

    void render(sf::RenderWindow &window);
};
