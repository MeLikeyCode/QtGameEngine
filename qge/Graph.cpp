#include "Graph.h"

#include "Tree.h"

using namespace qge;

/// Default constructor, constructs a Graph with no Nodes or Edges (empty Graph).
Graph::Graph(){

}

/// Constructs a Graph from the given set of Nodes and Edges.
///
/// Please ensure that all of the Nodes of the Edges are actually Nodes in the set of Nodes. In other words,
/// please make sure that the set of Nodes and Edges being passed in actually represent a graph. This constructor
/// does not do any checking.
Graph::Graph(const std::unordered_set<Node> &nodes, const std::unordered_set<Edge> &edges): nodes_(nodes),edges_(edges){

}

/// Adds the specified Node to the Graph.
///
/// The Node will not be added if an equivalent one is already in the Graph.
void Graph::addNode(const Node &node){
    nodes_.insert(node);
}

/// Adds an Edge between the specified Nodes.
///
/// The Edge will not be added if an equivalent one is already added in the Graph.
void Graph::addEdge(const Node &from, const Node &to,  int weight){
    // make sure from and to nodes exist in the graph
    assert(contains(from));
    assert(contains(to));

    // construct an edge
    Edge edge(from,to,weight);

    // add the edge to the set of edges
    edges_.insert(edge);
}

/// Returns a set of all the Nodes in the Graph.
std::unordered_set<Node> Graph::nodes() const{
    return nodes_;
}

/// Returns a set of all the Edges in the Graph.
std::unordered_set<Edge> Graph::edges() const{
    return edges_;
}

/// Returns a vector of all the outgoing Edges from the specified Node.
///
/// Outgoing edges are those that _start_ at the specified Node (and end on any other Node).
///
/// @param from The Node to get all the outoing Edges of.
std::vector<Edge> Graph::outgoingEdges(const Node &from) const{
    // make sure the from Node actually exists
    assert(contains(from));

    std::vector<Edge> edges;
    for(auto edge : edges_){
        if (edge.from() == from){
            edges.push_back(edge);
        }
    }
    return edges;
}

/// Returns a vector of all the incoming Edges to the specified Node.
///
/// Incoming edges are those that _end_ at the specified Node (can start on any other Node).
///
/// @param to The Node to get all the incoming Edges of.
std::vector<Edge> Graph::incomingEdges(const Node &to) const{
    // make sure the to Node actually exists
    assert(contains(to));

    std::vector<Edge> edges;
    for(auto edge : edges_){
        if (edge.to() == to){
            edges.push_back(edge);
        }
    }
    return edges;
}

/// Returns a vector of all the _adjacent_ Nodes that this Node can go to.
///
/// I.e. all the Nodes that this Node has an outoing Edge to.
///
/// @param from The Node to get the adjacent outgoing Nodes of.
std::vector<Node> Graph::outgoingNodes(const Node &from) const{
    // make sure the from Node exists
    assert(contains(from));

    // get the outgoing edges
    std::vector<Edge> outgoingE = outgoingEdges(from);

    // create a vector of the outgoing nodes
    std::vector<Node> outgoingN;
    for (auto edge:outgoingE){
        outgoingN.push_back(edge.to());
    }

    return outgoingN;
}

/// Returns a vector of all the _adjacent_ Nodes that can come to this Node.
///
/// I.e all the Nodes that have an incoming Edge to this Node.
///
/// @param to The Node to get all the _adjacent_ incoming Nodes of.
std::vector<Node> Graph::incomingNodes(const Node &to) const{
    // make sure the to Node exists
    assert(contains(to));

    // get the incoming edges
    std::vector<Edge> incomingE = incomingEdges(to);

    // create a vector of the incoming nodes
    std::vector<Node> incomingN;
    for (auto edge:incomingE){
        incomingN.push_back(edge.from());
    }

    return incomingN;
}

/// Returns true if the Graph contains the specified Node.
bool Graph::contains(const Node &node) const{
    return (nodes_.count(node) == 1); // sets have either 1 of each value or 0
}

/// Returns true if the Graph contains the specified Edge.
bool Graph::contains(const Edge &edge) const{
    return (edges_.count(edge) == 1); // sets have either 1 of each value or 0
}

/// Returns a vector of Nodes that represent the shortest path between the specified Nodes.
/// Uses A* pathfinding algorithm.
std::vector<Node> Graph::shortestPath(const Node &from, const Node &to) const{
    // make sure from and to are in the graph
    assert(contains(from));
    assert(contains(to));

    // return spt(from).pathTo(to);

    // consider lazy evaluation (retrieving stored value, short circuiting, etc..)
    // for functions that are expensive
    // GUARD: short circuit if to == from, just return an empty vector
    if (from == to){
        return std::vector<Node>();
    }

    // initialize variables
    openNodes_.clear();
    closedNodes_.clear();
    nodeToParent_.clear();
    nodeToFCost_.clear();
    nodeToGCost_.clear();
    nodeToHCost_.clear();

    // calculate costs (H, G, and F)
    calculateCosts_(from,to);

    // add the start node ("from" to list of open nodes
    openNodes_.insert(from);

    // traverse through
    Node current;
    while (true){
        // set the current Node as the one with the lowest F cost
        current = nodeInOpenWithLowestFCost();
        openNodes_.erase(current);      // remove it from the open nodes
        closedNodes_.insert(current);   // add it to the closed nodes ("visited")

        // if current node is the target node, path has been found
        if (current == to){
            break;
        }

        // for each neighbor of the current node
        for (Node neighbor:outgoingNodes(current)){
            // if neighbor is in closed, skip it
            if (closedNodes_.count(neighbor) > 0){
                continue;
            }

            // if neighbor is in open, but the new path is shorter
            int originalGCostOfNeighbor = nodeToGCost_[neighbor];
            int newGCostOfNeighbor = nodeToGCost_[current] + 1;
            if (newGCostOfNeighbor < originalGCostOfNeighbor){
                // update the cost of neighbor
                nodeToGCost_[neighbor] = nodeToGCost_[current] + 1;
                nodeToFCost_[neighbor] = nodeToGCost_[neighbor] + nodeToHCost_[neighbor];

                // set parent of neighbor
                nodeToParent_[neighbor] = current;
            }

            // if neighbor is NOT in open
            if (openNodes_.count(neighbor) == 0){
                // set cost of neighbor
                nodeToGCost_[neighbor] = nodeToGCost_[current] + 1;
                nodeToFCost_[neighbor] = nodeToGCost_[neighbor] + nodeToHCost_[neighbor];

                // set parent of neighbor
                nodeToParent_[neighbor] = current;

                // add it to open
                openNodes_.insert(neighbor);
            }
        }
    }

    // at this point, current is the target Node, follow it back to the start
    std::vector<Node> path;
    path.push_back(current);
    while (nodeToParent_.count(current) > 0){   // while current Node has a parent
        path.push_back(nodeToParent_[current]); // add parent to path
        current = nodeToParent_[current];       // go one backwards
    }

    // reverse the path (because it goes back->front) and then return it
    std::reverse(std::begin(path),std::end(path));
    return path;
}

/// Returns a shortest path Tree rooted at the specified ("source") Node.
/// Uses dijkstras algorithm therefore it is O(mlog(n)).
Tree Graph::spt(const Node &source) const{
    // make sure the source Node exists
    assert(contains(source));

    // initialize variables
    pickedNodes_.clear();
    unpickedNodes_.clear();
    pickedEdges_.clear();
    unpickedEdges_.clear();
    nodeWeight_.clear();
    updatedEdge_.clear();

    // need to remember the first picked node as the root
    Node rootNode(0,0);

    // mark all nodes as unpicked
    unpickAll();

    // initialize weights
    initializeNodeWeights(source);

    // while there is an unpicked node
    bool isFirstNode = true;
    while (hasUnpickedNode()){
        // pick the one with the lightest weight
        Node lightest = lightestUnpickedNode();
        pick(lightest);

        // update its neighbors weights
        updateNeighborWeights(lightest);

        // if this is the first picked node, it should also be the root & don't pick any edges!
        if (isFirstNode){
            rootNode = lightest;
            isFirstNode = false;
            continue;
        }

        // pick its edge
        pickConnetedEdge(lightest);
    }

    // create a graph from the picked set of nodes and edges
    Graph graph(pickedNodes_,pickedEdges_);

    // create/return a tree from the graph
    return Tree(graph,rootNode);

}

/// Picks the specified unpicked Node.
///
/// A Graph maintains a set of picked and a set of unpicked Nodes, which are helper attributes to
/// finding the spt of the Graph. This function will remove the specified node from the unpicked set
/// and puts it in the picked set.
void Graph::pick(const Node &node) const{
    // make sure the node is in unpickedNodes_
    assert(unpickedNodes_.count(node) == 1);

    // make sure it is not already picked
    assert(!picked(node));

    // pick it
    pickedNodes_.insert(node); // add it to picked
    unpickedNodes_.erase(node); // remove it from unpicked

}

/// Picks the specified unpicked Edge.
///
/// A Graph maintains a set of picked and a set of unpicked Edges, which is a helper attribute to
/// finding the spt of the Graph. This function will remove the specified Edge from the unpicked set
/// and put it in the picked set.
void Graph::pick(const Edge &edge) const{
    // make sure its unpicked
    //assert(unpickedEdges_.count(edge) == 1);
    //assert(!picked(edge)); TODO find out why this breaks code!?

    // pick it
    pickedEdges_.insert(edge);
    unpickedEdges_.erase(edge);

}

/// Returns true if the specified Node is picked.
///
/// @see Graph::pick(const Node&)
bool Graph::picked(const Node &node) const{
    // make sure the Node exists
    assert(contains(node));

    return (pickedNodes_.count(node) == 1);
}

/// Returns true if the specified Edge is picked.
///
/// @see Graph::pick(const Edge&)
bool Graph::picked(const Edge &edge) const{
    // make sure the edge exists
    assert(contains(edge));

    return (pickedEdges_.count(edge) == 1);
}

/// Sets the weight of the specified Node.
///
/// A Graph maintains a mapping of Node:int so that every Node has a weight. This is a helper attribute to
/// finding the spt of the Graph.
void Graph::setWeight(const Node &of,  int weight) const{
    // make sure the of Node exists
    assert(contains(of));

    nodeWeight_[of] = weight;
}

/// Returns the weight of the specified Node.
///
/// @see Graph::setWeight(const Node&, int);
 int Graph::weight(const Node &of) const{
    // make sure the of Node exists
    assert(contains(of));

    int weight = nodeWeight_.find(of)->second; // find returns iterator to pair, pair.first is the key, pair.second is value
    return weight;
}

/// Returns the Edge between the specified Nodes.
///
/// Helper function for determining the spt of the Graph.
Edge Graph::edge(const Node &from, const Node &to) const{
    // make sure the from node and the to node exists
    assert(contains(from));
    assert(contains(to));

    for (Edge edge:edges_){
        if (edge.from() == from && edge.to() == to){
            return edge;
        }
    }

    // make sure an edge was found! (if it gets here, no edge was found)
    assert(true);
    return Edge(); // just here to supress compiler waring ("not all control
                   // paths return a value"). This is a private helper function,
                   // it only gets called by this class in an attempt to break
                   // up the function into a bunch of named little ones. This
                   // function will only be called when there really is an edge
                   // between the specified nodes. In other words, it will
                   // always return
}

/// Adds all Nodes of the Graph to the set of unpicked Nodes and adds all Edges
/// of the Graph to the set of unpicked Edges.
///
/// @see Graph::pick(const Node&)
/// @see Graph::pick(const Edge&)
void Graph::unpickAll() const{
    // unpick nodes
    for (Node node:nodes_){
        unpickedNodes_.insert(node);
    }

    // unpick edges
    for (Edge edge:edges_){
        unpickedEdges_.insert(edge);
    }
}

/// Gives source Node weight of 0 and all other nodes weight of 1000
void Graph::initializeNodeWeights(const Node &source) const{
    // make sure the source Node exists
    assert(contains(source));

    // Give source node weight of 0
    setWeight(source,0);

    // Give all other nodes weight of 1000
    for (Node node:nodes_){
        if (!(node == source)){
            setWeight(node,1000);
        }
    }
}

/// Returns true if there is at least one unpicked Node.
bool Graph::hasUnpickedNode() const{
    return (!unpickedNodes_.empty());
}

/// Returns the lightest unicked Node.
Node Graph::lightestUnpickedNode() const{
    // make sure there is at least one unpicked node
    assert(hasUnpickedNode());

    // assume the first one is the lightest
    Node lightest = *(unpickedNodes_.begin());

    // try to find a lighter one
    for (Node node: unpickedNodes_){
        if (weight(node) < weight(lightest)){
            lightest = node;
        }
    }

    return lightest;
}

/// Picks the appropriate Edge of the specified Node.
///
/// This is a helper function for determining the spt of the Graph. When a Node is picked,
/// there is an appropriate Edge that must also be picked, therefore whenever a Node is picked,
/// I keep track of the appropriate Edge, this funtion simply picks that Edge.
void Graph::pickConnetedEdge(const Node &of) const{
    Edge edge = updatedEdge_[of];
    pick(edge);

}

/// Returns a vector of the unpicked neighbors of the specified Node.
///
/// This is a helper function for determining the spt of the Graph.
std::vector<Node> Graph::unpickedNeighbors(const Node &of) const{
    // find all neighbors, see which ones are unpicked
    std::vector<Node> neighbors = outgoingNodes(of);

    std::vector<Node> unpicked;
    for (auto neighbor:neighbors){
        if (!picked(neighbor)){ // if unpicked
            unpicked.push_back(neighbor);
        }
    }

    return unpicked;

}

/// Updates the weights of the neighboring Nodes if the new weight is smaller.
///
/// This is a helper function for determining the spt of the Graph.
void Graph::updateNeighborWeights(const Node &of) const{
    // For each unpicked neighbor
    for (Node neighbor:unpickedNeighbors(of)){
        // find weight of edge to neighbor
        Edge edgeToNeighbor = edge(of,neighbor);
        int edgeWeight = edgeToNeighbor.weight();

        // find new weight to neighbor and old weight
        int newTotalWeight = nodeWeight_[of] + edgeWeight;
        int oldWeight = nodeWeight_[neighbor];

        if (newTotalWeight < oldWeight){
            setWeight(neighbor,newTotalWeight);
            // remember that this edge was used to get to this node
            updatedEdge_[neighbor] = edgeToNeighbor;
        }

    }
}

/// Returns the Node in openNodes_ with the lowest F cost. If two Nodes have the
/// equivalent F cost, will return the one with the lower H cost. If two
/// Nodes have the same F and H cost, returns an arbitrary one.
Node Graph::nodeInOpenWithLowestFCost() const
{
    // find the lowest f cost in open
    bool initialFCostSet = false;
    int lowestFCost;
    for (Node node:openNodes_){
        // assume initial one is lowest
        if (!initialFCostSet){
           lowestFCost = nodeToFCost_[node];
           initialFCostSet = true;
        }

        // if we find a lower one, set that as lowest
        if (nodeToFCost_[node] < lowestFCost){
            lowestFCost = nodeToFCost_[node];
        }
    }

    // get all Nodes that have this f cost
    std::vector<Node> lowestFCostNodes;
    for (Node node:openNodes_){
        if (nodeToFCost_[node] == lowestFCost){
            lowestFCostNodes.push_back(node);
        }
    }

    // return the one with lowest h cost
    bool initialHCostSet = false;
    Node lowestCost;
    for (Node node:lowestFCostNodes){
        if (!initialHCostSet){
            lowestCost = node;
            initialHCostSet = true;
        }

        // if lower, set this one
        if (nodeToHCost_[node] < nodeToHCost_[lowestCost]){
            lowestCost = node;
        }
    }

    // Node lowestCost is the one with lowest F cost (lowest H cost if multiple equal F costs)
    return lowestCost;
}

/// Calculates H, G, and F costs for each Node.
void Graph::calculateCosts_(Node from, Node to) const
{
    for (Node node:nodes_){
        nodeToHCost_[node] = abs(to.x() - node.x()) + abs(to.y() - node.y());
        nodeToGCost_[node] = abs(from.x() - node.x()) + abs(from.y() - node.y());
        nodeToFCost_[node] = nodeToGCost_[node] + nodeToHCost_[node];
    }
}
