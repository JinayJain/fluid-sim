#include <SFML/Graphics.hpp>

#include "sim.h"
#include <vector>
#include <cmath>
#include <iostream>

#define SIM_SIZE 150
#define SIM_SCALE 2
#define SIM_DT 0.2f
#define SIM_VISC 0
#define SIM_DIFF 0.0000001f

void renderSim(Sim &sim, sf::Uint8 *pixels)
{
    std::vector<float> &d = sim.getDensity();

    for (int i = 0; i < SIM_SIZE; i++)
    {
        for (int j = 0; j < SIM_SIZE; j++)
        {
            int idx = sim.idx(j, i);
            float val = fmin(1.0, d[idx] / 200.0f);
            int r = val * 255;
            int g = val * 255;
            int b = val * 255;
            int a = 255;

            int idx2 = (i * SIM_SIZE + j) * 4;
            pixels[idx2] = r;
            pixels[idx2 + 1] = g;
            pixels[idx2 + 2] = b;
            pixels[idx2 + 3] = a;
        }
    }
}

sf::Vector2i getMouseSimPos(sf::Window &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    mousePos.x /= SIM_SCALE;
    mousePos.y /= SIM_SCALE;
    return mousePos;
}

int main()
{
    Sim sim(SIM_SIZE, SIM_SIZE, SIM_DT, SIM_VISC, SIM_DIFF);

    sf::RenderWindow window(sf::VideoMode(SIM_SIZE * SIM_SCALE, SIM_SIZE * SIM_SCALE), "SFML works!");

    sf::Texture texture;
    texture.create(SIM_SIZE, SIM_SIZE);

    sf::Uint8 *pixels = new sf::Uint8[SIM_SIZE * SIM_SIZE * 4];

    sf::Sprite sprite(texture);
    sprite.setScale(2.0f, 2.0f);

    clock_t current_ticks, delta_ticks;

    // store the previous mouse position
    sf::Vector2i prevPos;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2i mousePos = getMouseSimPos(window);

        sim.addVelocity(mousePos.x, mousePos.y, (float)(mousePos.x - prevPos.x) / 10.0, (float)(mousePos.y - prevPos.y) / 10.0);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // add fluid density to mouse area
            sim.addDensity(mousePos.x, mousePos.y, 200.0f);
        }

        window.clear();

        current_ticks = clock();
        sim.step();
        delta_ticks = clock() - current_ticks;
        clock_t fps = CLOCKS_PER_SEC / delta_ticks;
        std::cout << fps << std::endl;
        
        renderSim(sim, pixels);
        texture.update(pixels);
        window.draw(sprite);
        window.display();
        
        prevPos = mousePos;
    }
}
