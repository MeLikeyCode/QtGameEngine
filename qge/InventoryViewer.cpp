#include "InventoryViewer.h"
#include "Inventory.h"
#include "ItemCell.h"
#include "Item.h"
#include "ScrollWindow.h"

using namespace qge;

/// Creates a default InventoryViewer which visualizes the specified Inventory.
InventoryViewer::InventoryViewer(Inventory *inventory):
    border_(15),
    paddingBWCells_(5),
    numCellsHorizontally_(3),
    numCellsVertically_(3),
    cellWidth_(64),
    cellHeight_(64),
    cellBackgroundIsColor_(false),
    cellBackgroundPixmap_(QPixmap(":/resources/graphics/misc/invcellbg.png")),
    inventory_(inventory),
    scrollWindow_(new ScrollWindow())
{
    // visualize
    setInventory(inventory);

    // defaults
    scrollWindow_->setBackgroundPixmap(QPixmap(":/resources/graphics/misc/invbg.png"));
}

QGraphicsItem *InventoryViewer::getGraphicsItem()
{
    return scrollWindow_->getGraphicsItem();
}

/// Sets the Inventory to be viewed by the InventoryViewer.
void InventoryViewer::setInventory(Inventory *inventory)
{
    // if currently viewing an inventory, stop viewing it
    if (inventory_ != nullptr){
        disconnect(inventory_,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        disconnect(inventory_,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        disconnect(inventory_,&Inventory::destroyed,this,&InventoryViewer::onInventoryDestroyed);
    }

    // listen for changes to this inventory
    if (inventory != nullptr){
        connect(inventory,&Inventory::itemAdded,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        connect(inventory,&Inventory::itemRemoved,this,&InventoryViewer::onItemAddedOrRemovedFromInventory);
        connect(inventory_,&Inventory::destroyed,this,&InventoryViewer::onInventoryDestroyed);
        inventory_ = inventory;
    }

    // draw
    draw_();
}

/// Returns the Inventory that the InventoryViewer is visualizing.
Inventory *InventoryViewer::inventory()
{
    return inventory_;
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
    scrollWindow_->setBackgroundColor(color);
}

/// Sets the background of the InventoryViewer to the specified QPixmap.
/// @see InventoryViewer::setBackgroundColor(const QColor& color) for
/// more info.
void InventoryViewer::setBackgroundPixmap(const QPixmap &pixmap)
{
    scrollWindow_->setBackgroundPixmap(pixmap);
}

/// Sets the background of each cell of the InventoryViewer to be the specified color.
void InventoryViewer::setCellBackgroundColor(const QColor &color)
{
    cellBackgroundIsColor_ = true;
    cellBackgroundColor_ = color;
    draw_();
}

/// Sets the background of each cell of the InventoryViewer to be the specified pixmap.
void InventoryViewer::setCellBackgroundPixmap(const QPixmap &pixmap)
{
    cellBackgroundIsColor_ = false;
    cellBackgroundPixmap_ = pixmap;
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

/// Returns the height of the InventoryViewer.
double InventoryViewer::height()
{
    double bgHeight = numCellsVertically_ * (cellHeight_ + paddingBWCells_) - paddingBWCells_ + 2 * border_;
    return bgHeight;
}

/// Returns the width of the InventoryViewer.
double InventoryViewer::width()
{
    double bgWidth = numCellsHorizontally_ * (cellWidth_ + paddingBWCells_) - paddingBWCells_ + 2 * border_;
    return bgWidth;
}

/// Executed when an Item is added or removed from the InventoryViewer's Inventory.
void InventoryViewer::onItemAddedOrRemovedFromInventory(Item *item)
{
    setInventory(inventory_);
}

/// Executed when one of the ItemCells of the InventoryViewer are clicked.
/// Will emit a signal telling which item was just clicked.
void InventoryViewer::onItemCellClicked(ItemCell *itemCell, int button)
{
    Item* item = itemCell->item();
    if (item != nullptr){
        emit itemClicked(item,button);
    }
}

/// Executed when the Inventory of the InventoryViewer is destroyed.
/// Will simply redraw the InventoryViewer to reflect the fact that it does
/// not have an Inventory to view.
void InventoryViewer::onInventoryDestroyed()
{
    draw_();
}

/// Draws the Inventory based on its current states.
void InventoryViewer::draw_()
{
    // draw background
    double bgWidth = width();
    double bgHeight = height();

    scrollWindow_->setWidth(bgWidth);
    scrollWindow_->setHeight(bgHeight);

    // clear/stop listening to all previously drawn inventory cells
    for (ItemCell* cell:cells_){
        disconnect(cell,&ItemCell::clicked,this,&InventoryViewer::onItemCellClicked);
        scrollWindow_->remove(cell);
    }
    cells_.clear();

    // draw all items in new inventory cells
    if (inventory_ != nullptr){
        double x = 0;
        double y = 0;
        for (Item* item:inventory_->getItems()){
            ItemCell* cell = new ItemCell(cellWidth_,cellHeight_,item);
            if (cellBackgroundIsColor_)
                cell->setBackgroundColor(cellBackgroundColor_);
            else
                cell->setBackgroundPixmap(cellBackgroundPixmap_);
            double cellX = x*(cellWidth_+paddingBWCells_)+border_;
            double cellY = y*(cellHeight_+paddingBWCells_)+border_;
            cell->setGuiPos(QPointF(cellX,cellY));
            cells_.push_back(cell);
            scrollWindow_->add(cell,QPointF(cellX,cellY));

            connect(cell,&ItemCell::clicked,this,&InventoryViewer::onItemCellClicked);

            x++;
            if (x == numCellsHorizontally_){
                y++;
                x = 0;
            }
        }
    }
}
