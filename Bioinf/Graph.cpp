#pragma once

#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"


template<class Node, class Connection>
Graph<Node, Connection>::Graph(std::vector<Node> nodes, std::function<Connection(Node, Node)> mapper) : m_nodes(nodes) {
	int nodeAmount = nodes.size();
	int connectionAmount = nodeAmount * nodeAmount;
	m_connections = new Connection[connectionAmount];
	for (int x = 0; x < nodeAmount; x++) {
		for (int y = 0; y < nodeAmount; y++) {
			if (x != y) {
				Connection connection = mapper(getNode(x), getNode(y));
				setConnection(x, y, connection);
			}
		}
	}
}

template<typename Node, typename Connection>
void Graph<Node, Connection>::setConnection(int x, int y, Connection connection) {
	m_connections[index(x, y)] = connection;
}

template<typename Node, typename Connection>
Connection& Graph<Node, Connection>::getConnection(int x, int y) {
	return m_connections[index(x, y)];
}

template<typename Node, typename Connection>
Node Graph<Node, Connection>::getNode(int x) {
	return m_nodes[x];
}

template<typename Node, typename Connection>
int Graph<Node, Connection>::getSize() {
	return m_nodes.size();
}

template<typename Node, typename Connection>
int Graph<Node, Connection>::index(int x, int y) {
	return y * m_nodes.size() + x;
}

#endif