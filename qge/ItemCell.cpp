#include "ItemCell.h"

#include "Item.h"
#include "Inventory.h"
#include "Panel.h"
#include "Sprite.h"
#include "EntitySprite.h"

using namespace qge;

/// Constructs an ItemCell with the given width, height, and Item.
ItemCell::ItemCell(int width, int height, Item *item):
    background_(new Panel()),
    picture_(new QGraphicsPixmapItem(background_->getGraphicsItem())),
    numChargesText_(new QGraphicsSimpleTextItem(picture_.get())),
    item_(item)
{   
    // set bg width/height
    background_->setWidth(width);
    background_->setHeight(height);

    // listen to when the background emits clicked event
    connect(background_.get(),&Panel::clicked,this,&ItemCell::onBGPanelClicked_);

    setItem(item);
}

/// Sets the Item that the ItemCell should be visualizing.
void ItemCell::setItem(Item *item)
{
    // stop listening to previous item's 'num charges changed' event
    if (item_)
        disconnect(item_,&Item::numChargesChanged,this,&ItemCell::onItemChargesChanged_);

    // listen to new item's 'num charges changed' event
    item_ = item;
    connect(item_,&Item::numChargesChanged,this,&ItemCell::onItemChargesChanged_);

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
void ItemCell::onBGPanelClicked_(Panel* panel, QPointF pos, int button)
{
    emit clicked(this, button);
}

/// Executed when the number of charges of the ItemCell's Item changes.
/// Will update the text to display the current number of charges.
void ItemCell::onItemChargesChanged_(Item *sender)
{
    numChargesText_->setText(QString::number(sender->numOfCharges()));
}

/// Draws the ItemCell in its current state.
void ItemCell::draw_()
{
    // draw blank
    picture_->setPixmap(QPixmap());

    // draw item's picture (if has item)
    if (item_){
        double width = background_->width();
        double height = background_->height();
        if (item_->icon().isNull())
            picture_->setPixmap(item_->sprite()->currentlyDisplayedFrame().scaled(width-20,height-20));
        else
            picture_->setPixmap(item_->icon().scaled(width-20,height-20));
        picture_->setPos(10,10);

        numChargesText_->setText(QString::number(item_->numOfCharges()));
        numChargesText_->setBrush(QBrush(QColor(Qt::white)));
        numChargesText_->setPos(width-25,height-30);
        numChargesText_->setZValue(picture_->zValue() + 1);
    }
}
