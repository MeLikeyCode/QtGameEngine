#include <QGraphicsRectItem>
#include <QBrush>
#include <QtMath>
#include <unordered_set>
#include <iterator>
#include <QPointF>
#include <QGraphicsScene>

#include "InventoryViewer.h"
#include "Inventory.h"
#include "InventoryCell.h"
#include "Item.h"


/// Creates a new InventoryViewer of the specified size and visualizing the
/// specified Inventory.
InventoryViewer::InventoryViewer(Game* game, int width, int height, Inventory *inventory):
    Gui(new QGraphicsRectItem()),
    width_(width),
    height_(height),
    inventory_(inventory),
    cells_(),
    game_(game)
{
    // make sure Inventory viewer is high on top
    QGraphicsRectItem* asRI = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);
    asRI->setZValue(100);

    QBrush brush;
    brush.setColor(QColor(0,0,200,125));
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    asRI->setBrush(brush);
    asRI->setRect(0,0,width,height);

    // visualize contents of inventory
    if (inventory != nullptr){
        setInventory(inventory);
    }
}

void InventoryViewer::setInventory(Inventory *inventory)
{
    // if currently viewing an inventory, stop
    if (inventory_ != nullptr){
        disconnect(inventory_,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        disconnect(inventory_,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        for (InventoryCell* cell:cells_){
            QGraphicsScene* inScene = graphicsItem_->scene();
            if (inScene != nullptr){
                graphicsItem_->scene()->removeItem(cell);
            }
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
                InventoryCell* cell = new InventoryCell(game_,CELL_WIDTH,CELL_HEIGHT,*itemIterator,graphicsItem_);
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

    // listen for changes to this inventory
    connect(inventory,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
    connect(inventory,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
}

/// Executed when an Item is added to the InventoryViewer's Inventory.
void InventoryViewer::onItemAddedOrRemovedFromInventory(Item *item)
{
    setInventory(inventory_);
}
