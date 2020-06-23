#pragma once

#include "Solver.h"
#include "Graph.h"
#include <list>

class AntSolver :
	public Solver {
private:

	struct Connection {
		int cost;
		float pheromone;
		float tempPheromone;
		float probability;
	};

	struct Node {
		int index;
		std::string value;
	};

	// Inherited via Solver
	virtual void solve(std::vector<std::string> nodes, int n, int maxLength, bool onlyPositive, bool onlyNegative) override;

	std::vector<Node> initNodes(std::vector<std::string> nukleotydes);
	int calculateCost(std::string left, std::string right);
	int findStartingNodeIndex(Graph<Node, Connection>& graph);
	void updateProbability(Graph<Node, Connection>& graph);
	void addPheromones(Graph<Node, Connection>& graph, std::list<Node>& solution);
	void swapPheromones(Graph<Node, Connection>& graph);
	void decreasePheromones(Graph<Node, Connection>& graph);
	void printSolution(std::list<Node> solution, int maxSize);
};

