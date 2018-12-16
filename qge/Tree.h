#pragma once

#include "Vendor.h"

#include "Graph.h"
#include "Node.h"

namespace qge{

/// Represents a tree.
/// @author Abdullah Aghazadah
/// @date 4-24-15
///
/// A Tree is a Graph with no cycles.
class Tree{
public:
    // constructors
    Tree(const Node& root); // constructor must be given a node
    Tree(const Graph& graph, const Node& root);

    // readers ("getters")
    std::vector<Node> pathTo(const Node& node) const;
    std::unordered_set<Node> nodes() const;
    std::unordered_set<Edge> edges() const;

    // modifiers ("setters")
    void addChild(const Node& to, const Node& child, int weight);

private:
    // main private attributes
    Graph graph_;
    Node root_;

    // private helper attributes
    mutable std::unordered_set<Node> visitedNodes_;

    // private helper methods
    void visit(const Node& node) const;
    bool isVisited(const Node& node) const;
    bool hasUnvisitedChild(const Node& node) const;
    Node anUnvisitedChild(const Node& of) const;
    std::vector<Node> dfs(const Node& node, const Node& target, std::vector<Node> path) const;
};

}
