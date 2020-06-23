// Bioinf.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "AntSolver.h"

int main()
{
    AntSolver solver;

    for (const auto& entry : std::filesystem::directory_iterator("Instances")) {
        solver.run(entry.path().generic_string());
    }
}
