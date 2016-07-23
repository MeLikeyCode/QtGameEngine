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
InventoryViewer::InventoryViewer(Game* game, Inventory *inventory):
    Gui(new QGraphicsRectItem()),
    border_(15),
    paddingBWCells_(5),
    backgroundColor_(Qt::blue),
    numCellsHorizontally_(2),
    numCellsVertically_(3),
    cellWidth_(64),
    cellHeight_(64),
    inventory_(inventory),
    game_(game)
{
    rectItem_ = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);

    // make sure Inventory viewer is high on top
    rectItem_->setZValue(100);

    // visualize
    setInventory(inventory);
}

/// Sets the Inventory to be viewed by the InventoryViewer.
/// Throws an exception if the inventory is too big (i.e. has too many items) for
/// the specified size of the InventoryViwer. You can always increase the size
/// of the InventoryViwer if you need to view a bigger Inventory.
void InventoryViewer::setInventory(Inventory *inventory)
{
    // if currently viewing an inventory, stop viewing it
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

    // throw an exception if inventory is too big
    if (inventory!= nullptr && inventory->getItems().size() > numCellsHorizontally_ * numCellsVertically_){
        throw std::invalid_argument("inventory is too big to be visualized by this InventoryViewer. Resize the InventoryViwer first.");
    }

    // listen for changes to this inventory
    if (inventory != nullptr){
        connect(inventory,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        connect(inventory,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
    }

    // draw
    draw_();
}

/// Sets the distance between the outter edge of the InventoryViewer and the actual cells.
void InventoryViewer::setBorder(double amount)
{
    border_ = amount;
    draw_();
}

/// Sets the distance between the cells of the InventoryViewer.
void InventoryViewer::setPaddingBWCells(double amount)
{
    paddingBWCells_ = amount;
    draw_();
}

/// Sets the background color of the InventoryViewer.
/// Remember that you can include opacity information in the color.
void InventoryViewer::setBackgroundColor(const QColor &color)
{
    backgroundColor_ = color;
    draw_();
}

/// Sets the number of cells that the InventoryViewer has horizontally.
void InventoryViewer::setNumCellsHorizontally(int numCellsH)
{
    numCellsHorizontally_ = numCellsH;
    draw_();
}

/// Sets the number of cells that the InventoryViewer has vertically.
void InventoryViewer::setNumCellsVertically(int numCellsV)
{
    numCellsVertically_ = numCellsV;
    draw_();
}

/// Sets the width of each cell of the InventoryViewer.
void InventoryViewer::setCellWidth(double width)
{
    cellWidth_ = width;
    draw_();
}

/// Sets the height of each cell of the InventoryViewer.
void InventoryViewer::setCellHeight(double height)
{
    cellHeight_ = height;
    draw_();
}

/// Executed when an Item is added to the InventoryViewer's Inventory.
void InventoryViewer::onItemAddedOrRemovedFromInventory(Item *item)
{
    setInventory(inventory_);
}

/// Draws the Inventory based on its current states.
void InventoryViewer::draw_()
{
    // draw background
    double bgWidth = numCellsHorizontally_ * (cellWidth_ + paddingBWCells_) - paddingBWCells_ + 2 * border_;
    double bgHeight = numCellsVertically_ * (cellHeight_ + paddingBWCells_) - paddingBWCells_ + 2 * border_;
    rectItem_->setRect(0,0,bgWidth,bgHeight);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(backgroundColor_);
    rectItem_->setBrush(brush);

    // clear all items
    for (InventoryCell* cell:cells_){
        QGraphicsScene* inScene = graphicsItem_->scene();
        if (inScene != nullptr){
            graphicsItem_->scene()->removeItem(cell);
        }
    }

    // draw all items
    if (inventory_ != nullptr){
        double x = 0;
        double y = 0;
        for (Item* item:inventory_->getItems()){
            InventoryCell* cell = new InventoryCell(game_,cellWidth_,cellHeight_,item,rectItem_);
            cell->setX(x*(cellWidth_+paddingBWCells_)+border_);
            cell->setY(y*(cellHeight_+paddingBWCells_)+border_);
            cells_.push_back(cell);

            x++;
            if (x == numCellsHorizontally_){
                y++;
                x = 0;
            }
        }
    }
}
