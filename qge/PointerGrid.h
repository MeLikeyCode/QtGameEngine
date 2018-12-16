#pragma once

#include "Vendor.h"

#include "Node.h"

namespace qge{

/// A grid of pointers of any type. This structure is useful if you want to have a bunch of
/// pointers arranged in a gridlike fashion (cells/columns). The top left cell of the grid is
/// (0,0), bottom right is (numPointersX - 1, numPointerY - 1)
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// PointerGrid<MyType> pointerGrid(3,3); // MyType can be any type for whom you want to store pointers for
/// pointerGrid.setPointerAtPos(0,0,myPointer);
/// pointer.contains(myPointer) // check if this pointer is in the PointerGrid
/// pointer.pointerAt(0,0) // get the pointer at position (0,0), returns nullptr if no pointer is there
/// pointer.positionOf(myPointer) // get the position of your pointer, throws if 'myPointer' isn't even in the grid
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// @author Abdullah Aghazadah
/// @date 3/10/17
template<typename PointerType>
class PointerGrid
{
public:
    PointerGrid(int numPointersX = 0, int numPointersY = 0);

    // setters
    void setPointerAtPos(int xPos, int yPos, PointerType* pointer);
    void setPointerAtPos(const Node& pos, PointerType* pointer);

    // getters
    PointerType* pointerAt(int xPos, int yPos) const;
    bool contains(PointerType* pointer) const;
    Node positionOf(PointerType* pointer) const;
    std::vector<PointerType*> pointers() const;

private:
    std::vector<PointerType*> pointers_;
    int numPointersX_;
    int numPointersY_;
};

/// Constructs a PointerGrid with the specified number of pointers in the x and y direction.
template<typename PointerType>
PointerGrid<PointerType>::PointerGrid(int numPointersX, int numPointersY):
    numPointersX_(numPointersX),
    numPointersY_(numPointersY)
{
    // initialize all pointers to nullptr
    for (int i = 0, n = numPointersX_* numPointersY_; i < n; i++){
        pointers_.push_back(nullptr);
    }
}

/// Sets the pointer at the specified x,y position of the PointerGrid.
template<typename PointerType>
void PointerGrid<PointerType>::setPointerAtPos(int xPos, int yPos, PointerType *pointer)
{
    // make sure xPos and yPos are within bounds
    assert(xPos < numPointersX_ && yPos < numPointersY_ && xPos >= 0 && yPos >= 0);

    int posInArray = yPos * numPointersX_ + xPos;
    pointers_[posInArray] = pointer;
}

/// Overload of setPointerAtPos(int,int,PointerType*).
template<typename PointerType>
void PointerGrid<PointerType>::setPointerAtPos(const Node &pos, PointerType *pointer)
{
    setPointerAtPos(pos.x(),pos.y(),pointer);
}

/// Returns the pointer at the specified position in the PointerGrid.
/// If there is no pointer at the specified position, returns nullptr.
/// If the position is outside the bounds, will return nullptr (because technically there is no pointer
/// outside its bounds).
template<typename PointerType>
PointerType *PointerGrid<PointerType>::pointerAt(int xPos, int yPos) const
{
    if (xPos < 0 || xPos >= numPointersX_ || yPos < 0 || yPos >= numPointersY_){
        return nullptr;
    }

    int posInArray = yPos * numPointersX_ + xPos;
    return pointers_[posInArray];
}

/// Returns true if the PointerGrid contains the specified pointer.
template<typename PointerType>
bool PointerGrid<PointerType>::contains(PointerType *pointer) const
{
    for (PointerType* eachPointer:pointers_){
        if (eachPointer == pointer ){
            return true;
        }
    }
    return false;
}

/// Returns the position of the specified pointer.
/// If the pointer is not in the PointerGrid, will throw.
/// Use contains() to check if a pointer is the PointerGrid.
template<typename PointerType>
Node PointerGrid<PointerType>::positionOf(PointerType *pointer) const
{
    assert(contains(pointer));

    int indexOfPointer;
    for (int i = 0, n = pointers_.size(); i < n; i++){
        if (pointers_[i] == pointer){
            indexOfPointer = i;
            break;
        }
    }

    int yPos = indexOfPointer / numPointersX_;
    int xPos = indexOfPointer % numPointersX_;
    return Node(xPos,yPos);
}

/// Returns a vector of all the pointers in the PointerGrid.
/// (order is top left to bottom right).
template<typename PointerType>
std::vector<PointerType *> PointerGrid<PointerType>::pointers() const
{
    std::vector<PointerType*> resultantPointers;
    for (PointerType* pointer:pointers_){
        if (pointer != nullptr){
            resultantPointers.push_back(pointer);
        }
    }
    return resultantPointers;
}

}
