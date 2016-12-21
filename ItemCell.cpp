#include "ItemCell.h"
#include "Item.h"
#include "Inventory.h"
#include <cassert>
#include <QPixmap>
#include <QImage>
#include <QGraphicsSceneMouseEvent>
#include "Panel.h"
#include "Sprite.h"

/// Constructs an ItemCell with the given width, height, and Item.
ItemCell::ItemCell(int width, int height, Item *item):
    background_(new Panel()),
    item_(item),
    picture_(new QGraphicsPixmapItem())
{
    picture_->setParentItem(background_->getGraphicsItem());
    background_->setWidth(width);
    background_->setHeight(height);
    connect(background_.get(),&Panel::clicked,this,&ItemCell::onClicked_);
    draw_();
}

/// Sets the Item that the ItemCell should be visualizing.
void ItemCell::setItem(Item *item)
{
    item_ = item;
    draw_();
}

/// Returns the Item of the ItemCell.
Item *ItemCell::item()
{
    return item_;
}

/// Sets the width and height of the ItemCell.
void ItemCell::setSize(int width, int height)
{
    background_->setWidth(width);
    background_->setHeight(height);
}

/// Sets the background color of the ItemCell.
/// Remember that opacity can be included in the color information.
/// If this function is called last, the background of the ItemCell will
/// be a color, however, if setBackgroundPixmap(const QPixmap&) is called last,
/// the background will be a pixmap.
void ItemCell::setBackgroundColor(const QColor &color)
{
    background_->setBackgroundColor(color);
}

/// Sets the background of the ItemCell to be a QPixmap.
/// @see setBackgroundColor(const QColor&) for more info.
void ItemCell::setBackgroundPixmap(const QPixmap &pixmap)
{
    background_->setBackgroundPixmap(pixmap);
}

QGraphicsItem *ItemCell::getGraphicsItem()
{
    return background_->getGraphicsItem();
}

/// Executed when the ItemCell is clicked.
/// Will simply emit clicked();
void ItemCell::onClicked_(Panel* panel, QPointF pos, int button)
{
    emit clicked(this, button);
}

/// Draws the ItemCell in its current state.
void ItemCell::draw_()
{
    // draw item's picture
    if (item_ != nullptr){
        double width = background_->width();
        double height = background_->height();
        picture_->setPixmap(item_->sprite()->currentFrame().scaled(width-20,height-20));
        picture_->setPos(10,10);
    }
}
