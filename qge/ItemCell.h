#pragma once

#include "Vendor.h"

#include "Item.h"
#include "Gui.h"
#include "Panel.h"

class QGraphicsItem;
class QGraphicsPixmapItem;
class QColor;
class QPixmap;
class QGraphicsSimpleTextItem;

namespace qge{

class Panel;

/// Represents a Gui that visualizes and allows mouse interaction with an Item.
/// When the Item in the ItemCell is clicked, ItemCell will emit a signal.
/// You can set the Item of the ItemCell via setItem(Item*) or by passing the Item
/// in the ItemCell constructor. There are several member functions provided
/// which allow you to modify the look/size of the ItemCell.
class ItemCell: public Gui
{
    Q_OBJECT
public:
    ItemCell(int width, int height, Item* item = nullptr);

    void setItem(Item* item);
    Item* item();

    void setSize(int width, int height);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);

    QGraphicsItem* getGraphicsItem();

public slots:
    void onBGPanelClicked_(Panel *panel, QPointF pos, int button);
    void onItemChargesChanged_(Item* sender);

signals:
    void clicked(ItemCell* itemCell, int button);

private:
    std::unique_ptr<Panel> background_;
    std::unique_ptr<QGraphicsPixmapItem> picture_;
    QGraphicsSimpleTextItem* numChargesText_;
    QPointer<Item> item_;

    void draw_();
};

}
