#include "Tree.h"

using namespace qge;

/// Constructs a Tree with just a root Node.
Tree::Tree(const Node &root): root_(root), graph_(){
    // add the root node to the graph
    graph_.addNode(root);
}

/// Constructs a Tree from a given Graph and root Node.
///
/// Please make sure that the Graph is actually a Tree (i.e. has no cycles).
/// This constructor will not check for validity of the graph.
Tree::Tree(const Graph &graph, const Node &root): graph_(graph), root_(root){

}

/// Adds a child to the specified Node.
void Tree::addChild(const Node &to, const Node &child, int weight){
    // make sure the to Node exists
    assert(graph_.contains(to));

    // add child to graph
    graph_.addNode(child);

    // add edge b/w the "to" node and the child
    graph_.addEdge(to,child,weight);
}

/// Returns the vector of Nodes that goes from the root Node to the specified Node.
std::vector<Node> Tree::pathTo(const Node &node) const{
    // initialze variables
    visitedNodes_.clear();

    std::vector<Node> empty;
    return dfs(root_,node,empty);
}

/// Returns a set of all the Nodes.
std::unordered_set<Node> Tree::nodes() const{
    return graph_.nodes();
}

/// Returns a set of all the Edges.
std::unordered_set<Edge> Tree::edges() const{
    return graph_.edges();
}

/// Recursive dfs. Used by Tree::pathTo(const Node&).
std::vector<Node> Tree::dfs(const Node &node, const Node &target, std::vector<Node> path) const{
    // mark node visited & add it to the vector
    visit(node);

    // if node == target, return path so far
    if (node == target){
        path.push_back(node);
        return path;
    }

    // if node doesn't have any unvisited children, remove it, and run dfs on previous node in path
    if (!hasUnvisitedChild(node)){
        Node lastNode = path.back();
        path.pop_back();
        return dfs(lastNode,target,path);
    }

    // if node has unvisited children, pick one and run dfs on it
    if (hasUnvisitedChild(node)){
        path.push_back(node);
        Node unvisited = anUnvisitedChild(node);
        return dfs(unvisited,target,path);
    }

}

/// Marks the Node as visited (adds the Node to the set of visited Nodes).
///
/// The Tree maintains a set of visited Nodes as a helper attribute to dfs.
void Tree::visit(const Node &node) const{
    assert(graph_.contains(node));

    visitedNodes_.insert(node);
}

/// Returns true if the specified Node is visited.
bool Tree::isVisited(const Node &node) const{
    return (visitedNodes_.count(node) == 1);
}

/// Returns true if the specified Node has at least one unvisited child.
bool Tree::hasUnvisitedChild(const Node &node) const{
    // get a list of all of the nodes children
    std::vector<Node> children = graph_.outgoingNodes(node);

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
Node Tree::anUnvisitedChild(const Node &of) const{
    // make sure it has an unvisited child!
    assert(hasUnvisitedChild(of));

    // get a list of all the childern of the node
    std::vector<Node> children = graph_.outgoingNodes(of);

    // return the 1st unvisited child
    for (auto child : children){
        if (!isVisited(child)){
            return child;
        }
    }

}
