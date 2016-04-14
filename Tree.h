#ifndef TREE_H
#define TREE_H

#include "Graph.h"
#include "Node.h"
#include <vector>

/// Represents a tree.
/// @author Abdullah Aghazadah
/// @date 4-24-15
///
/// A Tree, like a Graph, is set of Nodes and a set of Edges. The distinctive feature of a Tree is
/// that it does not contain any cycles.
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
    std::unordered_set<Node> visitedNodes_;

    // private helper methods
    void visit(const Node& node);
    bool isVisited(const Node& node);
    bool hasUnvisitedChild(const Node& node);
    Node anUnvisitedChild(const Node& of);
    std::vector<Node> dfs(const Node& node, const Node& target, std::vector<Node> path);

};

#endif // TREE_H
