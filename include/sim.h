#pragma once

#include "SFML/Graphics.hpp"

class Sim
{
    int width;
    int height;

    // Arrays are stored as 1D, but can be indexed in 2D with a macro

    /// Velocity at each point
    std::vector<sf::Vector2f> velocity;

    /// Density of a dye at each point
    std::vector<float> density;

    /// Diffuse the dye to make it spread out
    void diffuse();

    /// Apply the velocities
    void advect();

    /// Ensure that the fluid remins incompressible
    void project();

public:
    Sim(int width, int height);

    /// Retrieve the array index for a given x, y coordinate
    int idx(int x, int y);

    /// Render the simulation onto the SFML window
    void render(sf::RenderWindow &window);

    /// Simulate a single physics step
    void step();
};
