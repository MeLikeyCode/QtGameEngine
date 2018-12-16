#pragma once

#include "Vendor.h"

#include "Node.h"
#include "Edge.h"

namespace qge{

class Tree;

/// Represents a Graph.
/// @author Abdullah Aghazadah
/// @date 4-24-15
///
/// A Graph is a set of Nodes and Edges.
class Graph{
public:
    // constructors
    Graph();
    Graph(const std::unordered_set<Node>& nodes, const std::unordered_set<Edge>& edges);

    // readers (getters)
    std::unordered_set<Node> nodes() const;
    std::unordered_set<Edge> edges() const;
    std::vector<Edge> outgoingEdges(const Node& from) const;
    std::vector<Edge> incomingEdges(const Node& to) const;
    std::vector<Node> outgoingNodes(const Node& from) const; // all ADJACENT nodes this node can go to
    std::vector<Node> incomingNodes(const Node& to) const;   // all ADJACENT nodes that can come to this node
    bool contains(const Node& node) const;
    bool contains(const Edge& edge) const;
    std::vector<Node> shortestPath(const Node& from, const Node& to) const;
    Tree spt(const Node& source) const;

    // modifiers (setters)
    void addNode(const Node& node);
    void addEdge(const Node& from, const Node& to,  int weight);
private:
    // main private attributes
    std::unordered_set<Node> nodes_;
    std::unordered_set<Edge> edges_;

    // helper attributes for dijkstras
    mutable std::unordered_set<Node> pickedNodes_;
    mutable std::unordered_set<Node> unpickedNodes_;
    mutable std::unordered_set<Edge> pickedEdges_;
    mutable std::unordered_set<Edge> unpickedEdges_;
    mutable std::unordered_map<Node, int> nodeWeight_;
    mutable std::unordered_map<Node,Edge> updatedEdge_;

    // helper functions for dijkstras
    void pick(const Node& node) const;
    void pick(const Edge& edge) const;
    bool picked(const Node& node) const;
    bool picked(const Edge& edge) const;
    void setWeight(const Node& of,  int weight) const;
    int weight(const Node& of) const;
    Edge edge(const Node& from, const Node& to) const;
    void unpickAll() const;
    void initializeNodeWeights(const Node& source) const;
    bool hasUnpickedNode() const;
    Node lightestUnpickedNode() const;
    void pickConnetedEdge(const Node& of) const;
    std::vector<Node> unpickedNeighbors(const Node& of) const;
    void updateNeighborWeights(const Node& of) const;

    // helper attributes for A*
    mutable std::unordered_set<Node> openNodes_;
    mutable std::unordered_set<Node> closedNodes_;
    mutable std::unordered_map<Node,Node> nodeToParent_;    // mapping of a Node to its parent
    mutable std::unordered_map<Node,int> nodeToFCost_;      // mapping of a Node and its F cost
    mutable std::unordered_map<Node,int> nodeToGCost_;      // mapping of a Node and its G cost
    mutable std::unordered_map<Node,int> nodeToHCost_;      // mapping of a Node and its H cost

    // helper functions for A*
    Node nodeInOpenWithLowestFCost() const;
    void calculateCosts_(Node from, Node to) const;
};

}
