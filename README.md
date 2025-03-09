# riptide-optics

## Overview
Riptide Optics is a project focused on simulating optical systems using Geant4. This project aims to provide accurate and efficient simulations for various optical configurations.

## Requirements
- Geant4
- CMake
- Ninja

## Installation
1. Clone the repository:
  ```sh
  git clone https://github.com/yourusername/riptide-optics.git
  cd riptide-optics
  ```

2. Create a build directory and compile the project using CMake:
  ```sh
  cmake -S . -B build -DGeant4_DIR=~/g4/lib/cmake/Geant4/ -G"Ninja Multi-Config"
  cmake --build build --config Debug
  ```

## Usage
To run the simulation, execute the following command:
```sh
./build/simulate
```

This simulation produces a 2D histogram `output.root` with a matrix that records the optical photons.
