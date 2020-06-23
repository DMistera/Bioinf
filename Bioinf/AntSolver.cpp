#include "AntSolver.h"
#include "Parameters.h"
#include <random>
#include <ctime>
#include <iostream>

void AntSolver::solve(std::vector<std::string> nukleotydes, int n, int maxLength, bool onlyPositive, bool onlyNegative) {
	srand(0);

	// Prepare nodes
	std::vector<Node> nodes = initNodes(nukleotydes);

	// Create a graph
	std::function<Connection(Node, Node)> mapper = [&](Node left, Node right) -> Connection {
		int cost = calculateCost(left.value, right.value);
		if (onlyPositive) {
			if (cost >= 1) {
				cost = 9999999;
			}
		}
		return { calculateCost(left.value, right.value), 1.0f };
	};
	Graph<Node, Connection> graph(nodes, mapper);

	// Search for the best starting node
	int startingNodeIndex = findStartingNodeIndex(graph);

	std::list<Node> bestSolution;

	//Number of iterations
	for (int i = 0; i < I; i++) {
		//Update probability of all connections
		updateProbability(graph);

		//Number of ants
		#pragma omp parallel
		#pragma omp for schedule(dynamic)
		for (int m = 0; m < M; m++) {

			// Prepare solution
			std::list<Node> solution;

			// Insert starting node
			Node startingNode = graph.getNode(startingNodeIndex);
			solution.push_back(startingNode);

			// Prepare list of node indices that have not been inserted yet
			std::list<int> indices;
			for (int i = 0; i < graph.getSize(); i++) {
				if (i != startingNodeIndex) {
					indices.push_back(i);
				}
			}

			int totalCost = startingNode.value.length();
			int leftIndex = startingNodeIndex;
			int rightIndex = startingNodeIndex;
			while (solution.size() < graph.getSize()) {

				//Calculate sum of probabilities
				float probabilitySum = 0.0;
				for (int index : indices) {
					probabilitySum += graph.getConnection(index, leftIndex).probability;
					probabilitySum += graph.getConnection(rightIndex, index).probability;
				}

				float random = (float(rand()) / float((RAND_MAX)) * probabilitySum);
				float probabilityAccumulator = 0.0f;

				// Check left side
				for (std::list<int>::iterator iterator = indices.begin(); iterator != indices.end(); ++iterator) {
					probabilityAccumulator += graph.getConnection(*iterator, leftIndex).probability;
					if (probabilityAccumulator > random) {
						totalCost += graph.getConnection(*iterator, leftIndex).cost;
						if (totalCost <= n) {
							solution.push_front(graph.getNode(*iterator));
							leftIndex = *iterator;
							indices.erase(iterator);
							goto con;
						}
						else {
							goto end;
						}
					}
				}

				// Check right side
				for (std::list<int>::iterator iterator = indices.begin(); iterator != indices.end(); ++iterator) {
					probabilityAccumulator += graph.getConnection(rightIndex, *iterator).probability;
					if (probabilityAccumulator > random) {
						totalCost += graph.getConnection(rightIndex, *iterator).cost;
						if (totalCost <= n) {
							solution.push_back(graph.getNode(*iterator));
							rightIndex = *iterator;
							indices.erase(iterator);
							goto con;
						}
						else {
							goto end;
						}
					}
				}

				// Code should never reach here but due to floating point precision it sometimes does
				solution.push_back(graph.getNode(indices.back()));
				rightIndex = indices.back();
				indices.pop_back();

				end: break;
				con:;
			}

			if (solution.size() > bestSolution.size()) {
				if (!onlyNegative || (onlyNegative && solution.size() >= maxLength)) {
					#pragma omp critical
					bestSolution = solution;
				}
			}

			//Update feromones
			#pragma omp critical
			addPheromones(graph, solution);
		}



		//std::cout << "Iteration " << i << " completed. Current best:" << bestSolution.size() << "/" << maxLength << std::endl;

		if (bestSolution.size() == maxLength) {
			break;
		}
	}

	printSolution(bestSolution, maxLength);
}

std::vector<AntSolver::Node> AntSolver::initNodes(std::vector<std::string> nukleotydes) {
	std::vector<AntSolver::Node> result;
	int i = 0;
	for (std::string str : nukleotydes) {
		result.push_back({ i++, str });
	}
	return result;
}

int AntSolver::calculateCost(std::string left, std::string right) {
	int length = left.length();
	for (int i = 0; i < length; i++) {
		for (int j = i; j < length; j++) {
			if (left[j] != right[j - i]) {
				goto x;
			}
		}
		return i == 0 ? length : i;
		x:;
	}
	return length;
}

int AntSolver::findStartingNodeIndex(Graph<Node, Connection>& graph) {
	int result = -1;
	int minValue = 99999;
	for (int i = 0; i < graph.getSize(); i++) {
		int sum = 0;
		for (int j = 0; j < graph.getSize(); j++) {
			if (i != j) {
				sum += graph.getConnection(i, j).cost;
				sum += graph.getConnection(j, i).cost;
			}
		}
		if (sum < minValue) {
			result = i;
			minValue = sum;
		}
	}
	return result;
}

void AntSolver::updateProbability(Graph<Node, Connection>& graph) {
	for (int x = 0; x < graph.getSize(); x++) {
		for (int y = 0; y < graph.getSize(); y++) {
			if (x != y) {
				Connection& connection = graph.getConnection(x, y);
				connection.probability = pow(connection.pheromone, ALPHA) / pow(connection.cost, BETA);
			}
		}
	}
}

void AntSolver::addPheromones(Graph<Node, Connection>& graph, std::list<Node>& solution) {
	std::list<Node>::iterator it = solution.begin();
	while (true) {
		int i1 = (*it).index;
		it++;
		if (it == solution.end()) {
			break;
		}
		int i2 = (*it).index;
		Connection& connection = graph.getConnection(i1, i2);
		connection.pheromone += EPSILON * pow((float)(solution.size()) / (float)(graph.getSize()), DELTA) / connection.cost;
	}
}

void AntSolver::decreasePheromones(Graph<Node, Connection>& graph) {
	for (int x = 0; x < graph.getSize(); x++) {
		for (int y = 0; y < graph.getSize(); y++) {
			if (x != y) {
				Connection& connection = graph.getConnection(x, y);
				connection.pheromone *= GAMMA;
			}
		}
	}
}

void AntSolver::printSolution(std::list<Node> solution, int maxSize) {
	/*for (Node n : solution) {
		std::cout << n.value << std::endl;
	}*/
	float percent = 100.0f * (float)solution.size() / (float)maxSize;
	std::cout << "Solution includes " << solution.size() << " out of " << maxSize << " (" << percent << "%) nukleotydes." << std::endl;
}
