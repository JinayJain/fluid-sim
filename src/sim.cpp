#include "sim.h"
#include <SFML/Graphics.hpp>

Sim::Sim(int width, int height)
{
    this->width = width;
    this->height = height;
}

void Sim::render(sf::RenderWindow &window)
{
    sf::RectangleShape shape(sf::Vector2f(50, 50));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(sf::Vector2f(100, 100));
    window.draw(shape);
}
