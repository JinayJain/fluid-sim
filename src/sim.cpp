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
void Sim::setBoundary(int b, std::vector<float> &x)
{
    for (int i = 1; i < width - 1; i++)
    {
        x[idx(i, 0)] = b == 2 ? -x[idx(i, 1)] : x[idx(i, 1)];
        x[idx(i, height - 1)] = b == 2 ? -x[idx(i, height - 2)] : x[idx(i, height - 2)];
    }

    for (int j = 1; j < height - 1; j++)
    {
        x[idx(0, j)] = b == 1 ? -x[idx(1, j)] : x[idx(1, j)];
        x[idx(width - 1, j)] = b == 1 ? -x[idx(width - 2, j)] : x[idx(width - 2, j)];
    }

    x[idx(0, 0)] = 0.33f * (x[idx(1, 0)] + x[idx(0, 1)] + x[idx(0, 0)]);
    x[idx(0, height - 1)] = 0.33f * (x[idx(1, height - 1)] + x[idx(0, height - 2)] + x[idx(0, height - 1)]);
    x[idx(width - 1, 0)] = 0.33f * (x[idx(width - 2, 0)] + x[idx(width - 1, 1)] + x[idx(width - 1, 0)]);
    x[idx(width - 1, height - 1)] = 0.33f * (x[idx(width - 2, height - 1)] + x[idx(width - 1, height - 2)] + x[idx(width - 1, height - 1)]);
}

/// Linear solver using Gauss-Seidel
void Sim::linSolve(int b, std::vector<float> &x, std::vector<float> &x0, float a, float c, int iter)
{
    float cRecip = 1.0f / c;
    for (int k = 0; k < iter; k++)
    {
        for (int i = 1; i < width - 1; i++)
        {
            for (int j = 1; j < height - 1; j++)
            {
                x[idx(i, j)] = (x0[idx(i, j)] +
                                a * (x[idx(i - 1, j)] +
                                     x[idx(i + 1, j)] +
                                     x[idx(i, j - 1)] +
                                     x[idx(i, j + 1)] +
                                     x[idx(i, j)] +
                                     x[idx(i, j)])) *
                               cRecip;
            }
        }
        setBoundary(b, x);
    }
}

/// Diffuse array values
void Sim::diffuse(int b, std::vector<float> &x, std::vector<float> &x0, float diff, float dt, int iter)
{
    float a = dt * diff * (width - 2) * (height - 2);
    linSolve(b, x, x0, a, 1 + 6 * a, iter);
}

/// Project array values
void Sim::project(std::vector<float> &velocX, std::vector<float> &velocY, std::vector<float> &p, std::vector<float> &div, int iter) {}

/// Advect array values
void Sim::advect(int b, std::vector<float> &d, std::vector<float> &d0, std::vector<float> &velocX, std::vector<float> &velocY, float dt) {}
