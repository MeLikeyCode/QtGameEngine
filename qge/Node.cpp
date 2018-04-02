#include "Node.h"

using namespace qge;

/// Constructs a Node with values x = 0, and y = 0.
Node::Node(): x_(0), y_(0){

}

/// Constructs a Node with the specified x and y values.
Node::Node( int x,  int y): x_(x), y_(y){

}

/// Returns the x value of the Node.
int Node::x() const{
    return x_;
}

/// Returns the y value of the Node.
int Node::y() const{
    return y_;
}

/// Sets the x value of the Node.
void Node::setX( int x){
    x_ = x;
}

/// Sets the y value of the Node.
void Node::setY( int y){
    y_ = y;
}

/// Returns true if both Nodes have the same x and y values.
bool qge::operator==(const Node &lhs, const Node &rhs){
    return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
}

/// Returns true if the two Nodes do not have the same x and y values.
bool qge::operator!=(const Node &lhs, const Node &rhs)
{
    return !(qge::operator==(lhs,rhs));
}
