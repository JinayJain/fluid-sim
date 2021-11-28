#include "sim.h"
#include <cmath>
#include <vector>
#include <iostream>

Sim::Sim(int width, int height, float dt, float visc, float diff)
{
    this->width = width;
    this->height = height;
    this->dt = dt;
    this->visc = visc;
    this->diff = diff;

    velX.resize(width * height, 0.0f);
    velY.resize(width * height, 0.0f);
    oldVelX.resize(width * height, 0.0f);
    oldVelY.resize(width * height, 0.0f);

    density.resize(width * height, 0.0f);
    oldDensity.resize(width * height, 0.0f);
}

void Sim::step()
{
    diffuse(1, oldVelX, velX, visc, dt, 16);
    diffuse(2, oldVelY, velY, visc, dt, 16);

    project(oldVelX, oldVelY, velX, velY, 16);

    advect(1, velX, oldVelX, oldVelX, oldVelY, dt);
    advect(2, velY, oldVelY, oldVelX, oldVelY, dt);

    project(velX, velY, oldVelX, oldVelY, 16);

    diffuse(0, oldDensity, density, diff, dt, 16);
    advect(0, density, oldDensity, velX, velY, dt);
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
void Sim::project(std::vector<float> &velocX, std::vector<float> &velocY, std::vector<float> &p, std::vector<float> &div, int iter)
{
    for (int j = 1; j < height - 1; j++)
    {
        for (int i = 1; i < width - 1; i++)
        {
            div[idx(i, j)] = -0.5f * (velocX[idx(i + 1, j)] - velocX[idx(i - 1, j)] + velocY[idx(i, j + 1)] - velocY[idx(i, j - 1)]) / height;
            p[idx(i, j)] = 0;
        }
    }
    setBoundary(0, div);
    setBoundary(0, p);
    linSolve(0, p, div, 1, 6, iter);
    for (int j = 1; j < height - 1; j++)
    {
        for (int i = 1; i < width - 1; i++)
        {
            velocX[idx(i, j)] -= 0.5f * (p[idx(i + 1, j)] - p[idx(i - 1, j)]) * width;
            velocY[idx(i, j)] -= 0.5f * (p[idx(i, j + 1)] - p[idx(i, j - 1)]) * height;
        }
    }
    setBoundary(1, velocX);
    setBoundary(2, velocY);
}

/// Advect array values
void Sim::advect(int b, std::vector<float> &d, std::vector<float> &d0, std::vector<float> &velocX, std::vector<float> &velocY, float dt)
{
    float i0, i1, j0, j1;
    float dtx = dt * (width - 2);
    float dty = dt * (height - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float widthFloat = width;
    float heightFloat = height;
    float ifloat, jfloat;
    int j, i;
    for (j = 1, jfloat = 1; j < height - 1; j++, jfloat++)
    {
        for (i = 1, ifloat = 1; i < width - 1; i++, ifloat++)
        {
            tmp1 = dtx * velocX[idx(i, j)];
            tmp2 = dty * velocY[idx(i, j)];
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if (x < 0.5f)
                x = 0.5f;
            if (x > widthFloat + 0.5f)
                x = widthFloat + 0.5f;
            i0 = ::floorf(x);
            i1 = i0 + 1.0f;
            if (y < 0.5f)
                y = 0.5f;
            if (y > heightFloat + 0.5f)
                y = heightFloat + 0.5f;
            j0 = ::floorf(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;

            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;

            d[idx(i, j)] = s0 * (t0 * d0[idx(i0i, j0i)] + t1 * d0[idx(i0i, j1i)]) + s1 * (t0 * d0[idx(i1i, j0i)] + t1 * d0[idx(i1i, j1i)]);
        }
    }
    setBoundary(b, d);
}

std::vector<float> &Sim::getDensity()
{
    return density;
}

void Sim::addDensity(int x, int y, float amount)
{
    int loc = idx(x, y);
    if (loc < 0 || loc >= density.size())
        return;

    density[loc] += amount;
}

void Sim::addVelocity(int x, int y, float amountX, float amountY)
{
    int loc = idx(x, y);
    if (loc < 0 || loc >= velX.size())
        return;

    velX[loc] += amountX;
    velY[loc] += amountY;
}
