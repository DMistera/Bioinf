#include "Solver.h"
#include <fstream>
#include <iostream>

void Solver::run(std::string path) {
	std::vector<std::string> nodes = read(path);
	size_t minusPos = path.find('-');
	size_t plusPos = path.find('+');
	bool onlyPos = minusPos != std::string::npos;
	bool onlyNeg = plusPos != std::string::npos;
	size_t signPos = minusPos != std::string::npos ? minusPos : plusPos;
	std::string originalLengthStr = path.substr(path.find('.') + 1, signPos - path.find('.'));
	int originalLength = std::stoi(originalLengthStr);
	int n = originalLength + nodes[0].length() - 1;
	int maxLength = originalLength < nodes.size() ? originalLength : nodes.size();
	std::cout << path << ": ";
	solve(nodes, n, maxLength, onlyPos, onlyNeg);
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
