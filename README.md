# N-Body Problem

This is a C++ solver that solves the N-Body Problem using a fourth-order Runge Kutta, which I wrote as part of my Computing & Numerical Methods 2 module coursework in 2023, as specified [here](./other/assignment.pdf). This was my first programming project and I obtained 92% for this aspect of the coursework.

## Installation

Use ```make -o main main.cpp``` to compile the code, which will generate an executable to run.

## Usage

A ```parameters.txt``` is provided and is used as an input to the system. 

The first line contains the values of $G$, $T$, $\Delta t$ where $G$ is the gravitational constant (see [theory](#https://en.wikipedia.org/wiki/N-body_problem)), $T$ is the simulation time and $\Delta t$ is the time step.

Each subsequent line should describe a body in the system,containing its absolute initial position $x_i(0)$,$y_i(0)$, initial velocity $\dot{x}_i(0)$,$\dot{y}_i(0)$ and mass $m_i$.

For example, the ```parameters.txt``` file for an Earth-Moon system is: 

```
3 100 2
5 1 2.3 4.5 0.5
0.2 66 2 0.545 0.1
```

Results are outputted to a file ```output.txt```, which contains data in the following column format: Body index $i$, $t$, $x_i$, $y_i$, $\dot{x}_i$, $\dot{y}_i$.