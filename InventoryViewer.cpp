#include "InventoryViewer.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QtMath>
#include "Inventory.h"
#include "unordered_set"
#include "InventoryCell.h"
#include <iterator>
#include <QPointF>
#include <QGraphicsScene>
#include "Item.h"

/// Creates a new InventoryViewer of the specified size and visualizing the
/// specified Inventory.
InventoryViewer::InventoryViewer(int width, int height, Inventory *inventory):
    width_(width),
    height_(height),
    inventory_(inventory),
    pos_(QPointF(0,0))
{


    QBrush brush;
    brush.setColor(QColor(0,0,200,125));
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    setBrush(brush);
    setRect(0,0,width,height);

    // visualize contents of inventory
    if (inventory != nullptr){
        setInventory(inventory);
    }
}

void InventoryViewer::setInventory(Inventory *inventory)
{
    // TODO: handle the case of setting a NEW inventory
    // disconnect from old inventory
    if (inventory_ != nullptr){
        disconnect(inventory_,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        disconnect(inventory_,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        for (InventoryCell* cell:cells_){
            scene()->removeItem(cell); // TODO: bad idea, temporary, firgure out a better way
        }
    }

    int CELL_WIDTH = 64;
    int CELL_HEIGHT = 64;

    // number of cells that can fit x wise and y wise
    int numCellsX = qFloor(width_/CELL_WIDTH);
    int numCellsY = qFloor(height_/CELL_HEIGHT);

    std::unordered_set<Item*> theItems = inventory->getItems();
    std::unordered_set<Item*>::iterator itemIterator = theItems.begin();
    for (int row = 0; row < numCellsX; row++){
        for (int col = 0; col < numCellsY; col++){
            if (itemIterator != theItems.end()){
                // create cell
                InventoryCell* cell = new InventoryCell(CELL_WIDTH,CELL_HEIGHT,*itemIterator,this);
                cell->setX(row * CELL_WIDTH);
                cell->setY(col*CELL_HEIGHT);
                cells_.push_back(cell);

                // advance iterator
                itemIterator = std::next(itemIterator);
            }
            break;
        }
        if (itemIterator == theItems.end()){
            break;
        }
    }

    // listen for changes to this inventory // TODO: make sure to unlisten when diff inv is set
    connect(inventory,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
    connect(inventory,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
}

/// Sets the position of the InventoryViewer in view coordinates.
void InventoryViewer::setViewPos(QPointF p)
{
    pos_ = p;
}

/// Returns the view pos of the InventoryViewer.
QPointF InventoryViewer::viewPos()
{
    return pos_;
}

/// Executed when an Item is added to the InventoryViewer's Inventory.
void InventoryViewer::onItemAddedOrRemovedFromInventory(Item *item)
{
    setInventory(inventory_);
}
