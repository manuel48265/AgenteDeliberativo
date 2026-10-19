# Deliberative planning agent

A deliberative agent in C++ that plans complete routes in a 2D world with classic search algorithms,
from breadth-first search to A* with battery management and coordination with a second agent.
Assignment 2 of the Artificial Intelligence course at the University of Granada (2023/24). **It won
first place in the deliberative-agent category of the course's internal agents competition (2024).**

## The problem

The world is a grid of terrains with different battery costs, which depend on the items each agent
carries (a bikini makes water cheaper, sneakers make forest cheaper). The agent controls itself and,
from level 1, gives orders to a second agent, the collaborator. It must compute a plan, a sequence
of actions, that reaches a goal under increasingly demanding conditions:

| Level | Goal | Algorithm |
|---|---|---|
| 0 | Reach a cell with the fewest actions | Breadth-first search |
| 1 | Bring the collaborator to a cell with the fewest actions | Breadth-first search over joint states |
| 2 | Reach a cell with the least battery spent | Uniform-cost search |
| 3 | Bring the collaborator to a cell with the least battery spent | A* over joint states |
| 4 | Keep completing goals in a partially known world, recharging when needed | A* with replanning |

## My solution

All the planning code is in
[`Comportamientos_Jugador/jugador.cpp`](Comportamientos_Jugador/jugador.cpp); the simulator and the
rest of the code were provided by the course staff.

- **State design.** A different state per level: position and orientation of the agent and, when
  needed, of the collaborator, the items each one holds and the last order given, so each search only
  tracks what matters for its goal.
- **Cost functions.** The battery cost of every action on every terrain, with and without items.
- **Heuristics for A\*.** Estimates of the remaining cost from the distance to the goal and from
  precomputed battery-cost tables.
- **Level 4.** The agent fills in its map from its sensors as it moves, replans when needed, chooses
  whether to move itself or the collaborator, and plans detours to charging cells when the battery is
  low.

## Build and run (Linux)

```bash
git clone https://github.com/manuel48265/AgenteDeliberativo.git
cd AgenteDeliberativo
./install.sh        # installs dependencies and runs cmake and make
./practica2         # with graphical interface
./practica2SG       # without interface
```
