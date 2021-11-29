#include "sim.h"
#include "consts.h"

#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include <chrono>


int main()
{
    srand(time(NULL));
    Sim sim(SIM_SIZE, SIM_SIZE, SIM_DT, SIM_VISC, SIM_DIFF);
    int prevPosx = 0, prevPosy = 0;
    float fps = 0.0;
    int count = 0;
#pragma acc data copy(sim.velX [0:sim.width * sim.height], sim.velY [0:sim.width * sim.height],       \
                      sim.oldVelX [0:sim.width * sim.height], sim.oldVelY [0:sim.width * sim.height], \
                      sim.density [0:sim.width * sim.height], sim.oldDensity [0:sim.width * sim.height])
    {
        for (int mousePosx = 0; mousePosx < SIM_SIZE; mousePosx+=3) {
            for (int mousePosy = 0; mousePosy < SIM_SIZE; mousePosy+=3) {
                auto start = std::chrono::high_resolution_clock::now();
                sim.addVelocity(mousePosx, mousePosy, (float)(mousePosx - prevPosx) / 10.0, (float)(mousePosy - prevPosy) / 10.0);
                if (rand() % 10 < 5) {
                    // add fluid density to mouse area
                    sim.addDensity(mousePosx, mousePosy, 200.0f);
                }

                sim.step();
                prevPosx = mousePosx;
                prevPosy = mousePosy;

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;

                fps += (float) 1.0 / diff.count();
                count++;
            }
        }
        fps /= count;
        std::cout << "FPS " << fps << std::endl;
    }
}