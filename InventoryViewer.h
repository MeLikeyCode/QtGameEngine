#ifndef INVENTORYVIEWER_H
#define INVENTORYVIEWER_H

#include <QObject>
#include "Gui.h"
#include <QPixmap>
#include <QColor>

class Inventory;
class QPointF;
class ItemCell;
class Item;
class ScrollWindow;

/// Represents a GUI element that visualizes and allows mouse interaction with
/// an Inventory. When an item in the inventory viewer is clicked, the
/// InventoryViewer emits a signal.
class InventoryViewer : public QObject, public Gui
{
    Q_OBJECT
public:
    InventoryViewer(Inventory* inventory=nullptr);

    QGraphicsItem* getGraphicsItem();

    void setInventory(Inventory* inventory);

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

public slots:
    void onItemAddedOrRemovedFromInventory(Item* item);

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

    ScrollWindow* scrollWindow_;
    Inventory* inventory_;
    std::vector<ItemCell*> cells_;
    Game* game_;

    // helper functions
    void draw_();
};

#endif // INVENTORYVIEWER_H
