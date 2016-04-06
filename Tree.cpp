#include "Tree.h"
#include <cassert>

/// Constructs a Tree with just a root Node.
Tree::Tree(const MyNode &root): root_(root), graph_(){
    // add the root node to the graph
    graph_.addNode(root);
}

/// Constructs a Tree from a given Graph and root Node.
///
/// Please make sure that the Graph is actually a Tree (i.e. has no cycles).
/// This constructor will not check for validity of the graph.
Tree::Tree(const Graph &graph, const MyNode &root): graph_(graph), root_(root){

}

/// Adds a child to the specified Node.
void Tree::addChild(const MyNode &to, const MyNode &child, int weight){
    // make sure the to Node exists
    assert(graph_.contains(to));

    // add child to graph
    graph_.addNode(child);

    // add edge b/w the "to" node and the child
    graph_.addEdge(to,child,weight);
}

/// Returns the vector of Nodes that goes from the root Node to the specified Node.
std::vector<MyNode> Tree::pathTo(const MyNode &node) const{
    // since this is const, create a copy of the tree, and run dfs on the copy
    // (since dfs is a modifier function)
    Tree copy  = *this;
    std::vector<MyNode> empty;
    return copy.dfs(root_,node,empty);
}

/// Returns a set of all the Nodes.
std::unordered_set<MyNode> Tree::nodes() const{
    return graph_.nodes();
}

/// Returns a set of all the Edges.
std::unordered_set<Edge> Tree::edges() const{
    return graph_.edges();
}

/// Recursive dfs. Used by Tree::pathTo(const Node&).
std::vector<MyNode> Tree::dfs(const MyNode &node, const MyNode &target, std::vector<MyNode> path){
    // mark node visited & add it to the vector
    visit(node);

    // if node == target, return path so far
    if (node == target){
        path.push_back(node);
        return path;
    }

    // if node doesn't have any unvisited children, remove it, and run dfs on previous node in path
    if (!hasUnvisitedChild(node)){
        MyNode lastNode = path.back();
        path.pop_back();
        return dfs(lastNode,target,path);
    }

    // if node has unvisited children, pick one and run dfs on it
    if (hasUnvisitedChild(node)){
        path.push_back(node);
        MyNode unvisited = anUnvisitedChild(node);
        return dfs(unvisited,target,path);
    }

}

/// Marks the Node as visited (adds the Node to the set of visited Nodes).
///
/// The Tree maintains a set of visited Nodes as a helper attribute to dfs.
void Tree::visit(const MyNode &node){
    assert(graph_.contains(node));

    visitedNodes_.insert(node);
}

/// Returns true if the specified Node is visited.
bool Tree::isVisited(const MyNode &node){
    return (visitedNodes_.count(node) == 1);
}

/// Returns true if the specified Node has at least one unvisited child.
bool Tree::hasUnvisitedChild(const MyNode &node){
    // get a list of all of the nodes children
    std::vector<MyNode> children = graph_.outgoingNodes(node);

    // see if there is at least one unvisited child
    int numUnvisited = 0;
    for (auto child : children){
        if (!isVisited(child)){ // if the child is NOT visited, increment counter
            numUnvisited++;
        }
    }

    return (numUnvisited > 0);
}

/// Returns an unvisited child of the specified Node.
///
/// Which child is returned, is unpredictable.
MyNode Tree::anUnvisitedChild(const MyNode &of){
    // make sure it has an unvisited child!
    assert(hasUnvisitedChild(of));

    // get a list of all the childern of the node
    std::vector<MyNode> children = graph_.outgoingNodes(of);

    // return the 1st unvisited child
    for (auto child : children){
        if (!isVisited(child)){
            return child;
        }
    }

}
