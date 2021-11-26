#pragma once

#include <vector>

class Sim
{
    int width;
    int height;

    // Arrays are stored as 1D, but can be indexed in 2D with a macro

    /// X Velocity at each point
    std::vector<float> velX;
    /// Y Velocity at each point
    std::vector<float> velY;

    /// Old X velocity at each point
    std::vector<float> oldVelX;
    /// Old Y velocity at each point
    std::vector<float> oldVelY;

    /// Density of a dye at each point
    std::vector<float> density;

    /// Old density at each point
    std::vector<float> oldDensity;

    /// Sets boundary conditions
    void setBoundary(int b, std::vector<float> &x);

    /// Linear solver using Gauss-Seidel
    void linSolve(int b, std::vector<float> &x, std::vector<float> &x0, float a, float c, int iter);

    /// Diffuse array values
    void diffuse(int b, std::vector<float> &x, std::vector<float> &x0, float diff, float dt, int iter);

    /// Project array values
    void project(std::vector<float> &velocX, std::vector<float> &velocY, std::vector<float> &p, std::vector<float> &div, int iter);

    /// Advect array values
    void advect(int b, std::vector<float> &d, std::vector<float> &d0, std::vector<float> &velocX, std::vector<float> &velocY, float dt);

public:
    Sim(int width, int height);

    /// Retrieve the array index for a given x, y coordinate
    int idx(int x, int y);

    /// Simulate a single physics step
    void step();
};
