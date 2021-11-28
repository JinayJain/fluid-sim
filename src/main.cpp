#include "sim.h"
#include <cmath>
#include <iostream>

#define SIM_SIZE 150
#define SIM_SCALE 2
#define SIM_DT 0.2f
#define SIM_VISC 0
#define SIM_DIFF 0.0000001f

void renderSim(Sim &sim)
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
        }
    }
}

int main()
{
    Sim sim(SIM_SIZE, SIM_SIZE, SIM_DT, SIM_VISC, SIM_DIFF);
    clock_t current_ticks, delta_ticks;
    // store the previous mouse position
    int prevPosx = 0, prevPosy = 0;
    srand (time(NULL));

    while (1)
    {
        int mousePosx = rand() % SIM_SIZE;
        int mousePosy = rand() % SIM_SIZE;
        std::cout<<mousePosx <<", "<<mousePosy<<std::endl;
        sim.addVelocity(mousePosx, mousePosy, (float)(mousePosx - prevPosx) / 10.0, (float)(mousePosy - prevPosy) / 10.0);
        if (rand() % 10 < 5)
        {
            // add fluid density to mouse area
            sim.addDensity(mousePosx, mousePosy, 200.0f);
        }
        current_ticks = clock();
        sim.step();
        delta_ticks = clock() - current_ticks;
        clock_t fps = CLOCKS_PER_SEC / delta_ticks;
        //std::cout << fps << std::endl;
        
        renderSim(sim);
        prevPosx = mousePosx;
        prevPosy = mousePosy;
    }
}
