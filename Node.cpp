#include "Node.h"

/// Do not use. This is just here so Nodes can be stored in Maps.
MyNode::MyNode(){

}

/// Constructs a Node with the specified x and y values.
MyNode::MyNode( int x,  int y): x_(x), y_(y){

}

/// Returns the x value of the Node.
int MyNode::x() const{
    return x_;
}

/// Returns the y value of the Node.
int MyNode::y() const{
    return y_;
}

/// Sets the x value of the Node.
void MyNode::setX( int x){
    x_ = x;
}

/// Sets the y value of the Node.
void MyNode::setY( int y){
    y_ = y;
}

/// Returns true if both Nodes have the same x and y values.
bool operator==(const MyNode &lhs, const MyNode &rhs){
    return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
}
