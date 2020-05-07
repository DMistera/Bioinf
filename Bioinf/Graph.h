#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <functional>

template<typename Node, typename Connection>
class Graph {
public:
	Graph(std::vector<Node> nodes, std::function<Connection(Node, Node)> mapper);
	void setConnection(int x, int y, Connection connection);
	Connection& getConnection(int x, int y);
	Node getNode(int x);
	int getSize();
private:
	int index(int x, int y);
	std::vector<Node> m_nodes;
	Connection* m_connections;
};

#include "Graph.cpp"
#endif