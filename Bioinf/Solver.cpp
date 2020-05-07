#include "Solver.h"
#include <fstream>

void Solver::run(std::string path, int n) {
	std::vector<std::string> nodes = read(path);
	solve(nodes, n);
}

std::vector<std::string> Solver::read(std::string path) {
	std::ifstream file(path);
	if (file.is_open()) {
		std::vector<std::string> result;
		std::string line;
		while (std::getline(file, line)) {
			result.push_back(line);
		}
		return result;
	}
	else {
		throw std::exception("Could nott open file!");
	}
}
