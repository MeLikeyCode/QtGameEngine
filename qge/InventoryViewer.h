#pragma once

#include "Vendor.h"

#include "Gui.h"
#include "Inventory.h"
#include "ScrollWindow.h"

class QPointF;

namespace qge{

class ItemCell;
class Item;
class ScrollWindow;

/// Represents a GUI element that visualizes and allows mouse interaction with
/// an Inventory. When an item in the inventory viewer is clicked, the
/// InventoryViewer emits a signal.
class InventoryViewer : public Gui
{
    Q_OBJECT
public:
    InventoryViewer(Inventory* inventory=nullptr);

    QGraphicsItem* getGraphicsItem();

    void setInventory(Inventory* inventory);
    Inventory* inventory();

    void setBorder(double amount);
    void setPaddingBWCells(double amount);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);
    void setCellBackgroundColor(const QColor& color);
    void setCellBackgroundPixmap(const QPixmap& pixmap);
    void setNumCellsHorizontally(int numCellsH);
    void setNumCellsVertically(int numCellsV);
    void setCellWidth(double width);
    void setCellHeight(double height);

    double height();
    double width();

public slots:
    void onItemAddedOrRemovedFromInventory(Item* item);
    void onItemCellClicked(ItemCell* itemCell, int button);
    void onInventoryDestroyed();

signals:
    void itemClicked(Item* item, int button);

private:
    double border_;
    double paddingBWCells_;
    int numCellsHorizontally_;
    int numCellsVertically_;
    double cellWidth_;
    double cellHeight_;

    // cell options
    bool cellBackgroundIsColor_;
    QColor cellBackgroundColor_;
    QPixmap cellBackgroundPixmap_;

    std::unique_ptr<ScrollWindow> scrollWindow_;
    QPointer<Inventory> inventory_;
    std::vector<ItemCell*> cells_;

    // helper functions
    void draw_();
};

}
