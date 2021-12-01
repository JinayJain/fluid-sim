![demo](sim-demo.gif)

# Parallel Fluid Simulation

**Created by Ishika Govil and Jinay Jain**

## Introduction

This project is a parallelized implementation of the [Jos Stam paper](http://graphics.cs.cmu.edu/nsp/course/15-464/Fall09/papers/StamFluidforGames.pdf) on realtime fluid simulations. We use SFML, OpenACC, and C++ to implement the simulation and achieve a high performance visualization.

Inspired by a [serial implementation](https://github.com/Driema/euler-fluid-cpp) found on GitHub.

## Demo

Click to see a [YouTube demonstration](https://www.youtube.com/watch?v=wJig-ARfRPA) of the project in action.

[![Demo Video](https://img.youtube.com/vi/wJig-ARfRPA/0.jpg)](https://www.youtube.com/watch?v=wJig-ARfRPA)

## Requirements

The following tools are required to run the project:

- [NVIDIA HPC SDK v21.7](https://developer.nvidia.com/hpc-sdk)
- [SFML v2.5](http://www.sfml-dev.org/download.php)

## Installation

### Local Machine

Clone this project using [git](https://git-scm.com/).

```
git clone https://github.com/JinayJain/fluid-sim.git
```

Run the project using the Makefile

```
make run
```

### Bridges 2 Cluster

Clone this project using [git](https://git-scm.com/).

```
git clone https://github.com/JinayJain/fluid-sim.git
```

Switch to the branch without SFML dependencies using the following command:

```
git checkout without-sfml
```

Queue the batch job using the following command:

```bash
# For GPU Usage
sbatch test-script-gpu

# For CPU Usage
sbatch test-script-cpu
```

## Configuration

To change any parameters about the simulation, edit the `include/consts.h` file and recompile the project.

```bash
make clean
make run
```
