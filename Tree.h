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
    Tree(const MyNode& root); // constructor must be given a node
    Tree(const Graph& graph, const MyNode& root);

    // readers ("getters")
    std::vector<MyNode> pathTo(const MyNode& node) const;
    std::unordered_set<MyNode> nodes() const;
    std::unordered_set<Edge> edges() const;

    // modifiers ("setters")
    void addChild(const MyNode& to, const MyNode& child, int weight);

private:
    // main private attributes
    Graph graph_;
    MyNode root_;

    // private helper attributes
    std::unordered_set<MyNode> visitedNodes_;

    // private helper methods
    void visit(const MyNode& node);
    bool isVisited(const MyNode& node);
    bool hasUnvisitedChild(const MyNode& node);
    MyNode anUnvisitedChild(const MyNode& of);
    std::vector<MyNode> dfs(const MyNode& node, const MyNode& target, std::vector<MyNode> path);

};

#endif // TREE_H
