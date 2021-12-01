#include <SFML/Graphics.hpp>

#include "sim.h"
#include "consts.h"

#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include <chrono>

#include <openacc.h>

#define COLOR_R 0
#define COLOR_G 255
#define COLOR_B 255

void renderSim(Sim &sim, sf::Uint8 *pixels, sf::Vector2i mousePos, sf::Vector2i prevPos, bool pressed)
{
    float *d = sim.getDensity();
    float *vx = sim.velX;
    float *vy = sim.velY;

#pragma acc parallel loop present(d [0:SIM_SIZE * SIM_SIZE], vx [0:SIM_SIZE * SIM_SIZE], vy [0:SIM_SIZE * SIM_SIZE]) copy(pixels [0:SIM_SIZE * SIM_SIZE * 4])
    for (int i = 0; i < SIM_SIZE; i++)
    {
#pragma acc loop
        for (int j = 0; j < SIM_SIZE; j++)
        {
            int idx = i * SIM_SIZE + j;

            if (j == mousePos.x && i == mousePos.y && i > 0 && i < SIM_SIZE - 1 && j > 0 && j < SIM_SIZE - 1)
            {
                if (pressed)
                    d[idx] += 200.0f;

                vx[idx] += (float)(mousePos.x - prevPos.x) / 10.0;
                vy[idx] += (float)(mousePos.y - prevPos.y) / 10.0;
            }

            d[idx] = fmax(0, d[idx] - SIM_FADE);

            float val = fmin(1.0, d[idx] / 70.0f);
            int r = val * COLOR_R;
            int g = val * COLOR_G;
            int b = val * COLOR_B;
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
    srand(time(NULL));

    Sim sim(SIM_SIZE, SIM_SIZE, SIM_DT, SIM_VISC, SIM_DIFF);

    sf::RenderWindow window(sf::VideoMode(SIM_SIZE * SIM_SCALE, SIM_SIZE * SIM_SCALE), "Fluid Simulation");

    sf::Texture texture;
    texture.create(SIM_SIZE, SIM_SIZE);

    sf::Uint8 *pixels = new sf::Uint8[SIM_SIZE * SIM_SIZE * 4];

    sf::Sprite sprite(texture);
    sprite.setScale(SIM_SCALE, SIM_SCALE);

    // store the previous mouse position
    sf::Vector2i prevPos;

    std::vector<std::pair<sf::Vector2i, sf::Vector2f>> emitters;

    for (int i = 0; i < 40; i++)
    {
        emitters.push_back(std::make_pair(sf::Vector2i(rand() % SIM_SIZE, rand() % SIM_SIZE),
                                          sf::Vector2f(rand() % 3 - 1, rand() % 3 - 1)));
    }

#pragma acc data copy(sim.velX [0:sim.width * sim.height], sim.velY [0:sim.width * sim.height],       \
                      sim.oldVelX [0:sim.width * sim.height], sim.oldVelY [0:sim.width * sim.height], \
                      sim.density [0:sim.width * sim.height], sim.oldDensity [0:sim.width * sim.height])
    {
        while (window.isOpen())
        {
            auto start = std::chrono::high_resolution_clock::now();

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            sf::Vector2i mousePos = getMouseSimPos(window);

            // sim.addVelocity(mousePos.x, mousePos.y, (float)(mousePos.x - prevPos.x) / 10.0, (float)(mousePos.y - prevPos.y) / 10.0);

            window.clear();

            sim.step();

            renderSim(sim, pixels, mousePos, prevPos, sf::Mouse::isButtonPressed(sf::Mouse::Left));
            texture.update(pixels);
            window.draw(sprite);
            window.display();

            prevPos = mousePos;

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;

            std::cout << "FPS: " << 1.0 / diff.count() << std::endl;
        }
    }
}
