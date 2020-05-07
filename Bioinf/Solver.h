#pragma once

#include <string>
#include <vector>

class Solver {
public:
    void run(std::string path, int n);
private:
    std::vector<std::string> read(std::string path);
    virtual void solve(std::vector<std::string> nodes, int n) = 0;
};

