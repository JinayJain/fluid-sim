#pragma once

#include <vector>

class Sim
{
public:
    int width;
    int height;

    float dt;
    float visc;
    float diff;

    // Arrays are stored as 1D, but can be indexed in 2D with a macro

    /// X Velocity at each point
    float *velX;
    /// Y Velocity at each point
    float *velY;

    /// Old X velocity at each point
    float *oldVelX;
    /// Old Y velocity at each point
    float *oldVelY;

    /// Density of a dye at each point
    float *density;

    /// Old density at each point
    float *oldDensity;

    /// Sets boundary conditions
    void setBoundary(int b, float *x);

    /// Linear solver using Gauss-Seidel
    void linSolve(int b, float *x, float *x0, float a, float c, int iter);

    /// Diffuse array values
    void diffuse(int b, float *x, float *x0, float diff, float dt, int iter);

    /// Project array values
    void project(float *velocX, float *velocY, float *p, float *div, int iter);

    /// Advect array values
    void advect(int b, float *d, float *d0, float *velocX, float *velocY, float dt);

    Sim(int width, int height, float dt, float visc, float diff);

    /// Retrieve the array index for a given x, y coordinate
    int idx(int x, int y);

    /// Simulate a single physics step
    void step();

    /// Gets a reference to the density array for rendering
    float *getDensity();

    /// Adds density to a certain location in the array
    void addDensity(int x, int y, float amount);

    /// Adds velocity to a certain location in the array
    void addVelocity(int x, int y, float amountX, float amountY);
};