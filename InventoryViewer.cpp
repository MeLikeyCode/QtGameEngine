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
    border_(15),
    paddingBWCells_(5),
    backgroundColor_(Qt::blue),
    backgroundPixmap_(QPixmap(":/resources/graphics/misc/invbg.png")),
    backgroundIsPixmap_(true),
    numCellsHorizontally_(2),
    numCellsVertically_(3),
    cellWidth_(64),
    cellHeight_(64),
    inventory_(inventory),
    game_(game)
{
    pixmapItem_ = new QGraphicsPixmapItem();

    // visualize
    setInventory(inventory);
}

QGraphicsItem *InventoryViewer::getGraphicsItem()
{
    return pixmapItem_;
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
            QGraphicsScene* inScene = pixmapItem_->scene();
            if (inScene != nullptr){
                pixmapItem_->scene()->removeItem(cell);
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
/// Call this function last if you want the background of the InventoryViewer to
/// simply be a color, otherwise call setBackgroundPixmap last.
void InventoryViewer::setBackgroundColor(const QColor &color)
{
    backgroundColor_ = color;
    backgroundIsPixmap_ = false;
    draw_();
}

/// Sets the background of the InventoryViewer to the specified QPixmap.
/// @see InventoryViewer::setBackgroundColor(const QColor& color) for
/// more info.
void InventoryViewer::setBackgroundPixmap(const QPixmap &pixmap)
{
    backgroundPixmap_ = pixmap;
    backgroundIsPixmap_ = true;
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

    if (backgroundIsPixmap_){
        backgroundPixmap_ = backgroundPixmap_.scaled(bgWidth,bgHeight);
        pixmapItem_->setPixmap(backgroundPixmap_);
    } else{
        QImage img(QSize(bgWidth,bgHeight),QImage::Format_RGB32);
        img.fill(backgroundColor_);
        pixmapItem_->setPixmap(QPixmap::fromImage(img));
    }

    // clear all items
    for (InventoryCell* cell:cells_){
        QGraphicsScene* inScene = pixmapItem_->scene();
        if (inScene != nullptr){
            pixmapItem_->scene()->removeItem(cell);
        }
    }

    // draw all items
    if (inventory_ != nullptr){
        double x = 0;
        double y = 0;
        for (Item* item:inventory_->getItems()){
            InventoryCell* cell = new InventoryCell(game_,cellWidth_,cellHeight_,item,pixmapItem_);
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
