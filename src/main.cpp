#include <SFML/Graphics.hpp>

#include "sim.h"

#define SIM_SIZE 800

int main()
{
    Sim sim(SIM_SIZE, SIM_SIZE);

    sf::RenderWindow window(sf::VideoMode(SIM_SIZE, SIM_SIZE), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        sim.render(window);

        window.display();
    }
}
