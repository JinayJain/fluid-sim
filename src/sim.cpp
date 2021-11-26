#include "sim.h"

#include <vector>

Sim::Sim(int width, int height)
{
    this->width = width;
    this->height = height;

    velX.resize(width * height, 0.0f);
    velY.resize(width * height, 0.0f);
    oldVelX.resize(width * height, 0.0f);
    oldVelY.resize(width * height, 0.0f);

    density.resize(width * height, 0.0f);
    oldDensity.resize(width * height, 0.0f);
}

void Sim::step()
{
}

int Sim::idx(int x, int y)
{
    return x + y * width;
}

/// Sets boundary conditions
void Sim::setBoundary(int b, std::vector<float> &x) {}

/// Linear solver using Gauss-Seidel
void Sim::linSolve(int b, std::vector<float> &x, std::vector<float> &x0, float a, float c, int iter) {}

/// Diffuse array values
void Sim::diffuse(int b, std::vector<float> &x, std::vector<float> &x0, float diff, float dt, int iter) {}

/// Project array values
void Sim::project(std::vector<float> &velocX, std::vector<float> &velocY, std::vector<float> &p, std::vector<float> &div, int iter) {}

/// Advect array values
void Sim::advect(int b, std::vector<float> &d, std::vector<float> &d0, std::vector<float> &velocX, std::vector<float> &velocY, float dt) {}
