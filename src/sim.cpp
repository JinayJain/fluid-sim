#include "sim.h"

#include <SFML/Graphics.hpp>
#include <vector>

Sim::Sim(int width, int height)
{
    this->width = width;
    this->height = height;

    velocity.resize(width * height, sf::Vector2f(0, 0));
    density.resize(width * height, 0);
}

void Sim::render(sf::RenderWindow &window)
{
}

void Sim::step()
{
}

int Sim::idx(int x, int y)
{
    return x + y * width;
}
