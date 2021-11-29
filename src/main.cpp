#include "sim.h"
#include <cmath>
#include <iostream>

#define SIM_SIZE 150
#define SIM_SCALE 3
#define SIM_DT 0.2f
#define SIM_VISC 0
#define SIM_DIFF 0.0000001f

int main()
{
    Sim sim(SIM_SIZE, SIM_SIZE, SIM_DT, SIM_VISC, SIM_DIFF);
    clock_t current_ticks, delta_ticks;
    // store the previous mouse position
    int prevPosx = 0, prevPosy = 0;
    srand (time(NULL));
    float fps = 0.0;
    int count = 0;
    for (int mousePosx = 0; mousePosx < SIM_SIZE; mousePosx+=3) {
        for (int mousePosy = 0; mousePosy < SIM_SIZE; mousePosy+=3) {
            sim.addVelocity(mousePosx, mousePosy, (float)(mousePosx - prevPosx) / 10.0, (float)(mousePosy - prevPosy) / 10.0);
            if (rand() % 10 < 5) {
                // add fluid density to mouse area
                sim.addDensity(mousePosx, mousePosy, 200.0f);
            }
            current_ticks = clock();
            sim.step();
            delta_ticks = clock() - current_ticks;
            fps += (float) CLOCKS_PER_SEC / delta_ticks;
            prevPosx = mousePosx;
            prevPosy = mousePosy;
            count++;
        }
    }
    fps /= count;
    std::cout << "FPS " << fps << std::endl;
}
