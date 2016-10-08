#ifndef INVENTORYVIEWER_H
#define INVENTORYVIEWER_H

#include <QObject>
#include "Gui.h"
#include <QPixmap>

class Inventory;
class QPointF;
class InventoryCell;
class QGraphicsPixmapItem;
class Item;
class Game;

/// Represents a GUI element that visualizes and allows interaction with
/// an Inventory.
class InventoryViewer : public QObject, public Gui
{
    Q_OBJECT
public:
    InventoryViewer(Game* game, Inventory* inventory=nullptr);

    QGraphicsItem* getGraphicsItem();

    void setInventory(Inventory* inventory);

    void setBorder(double amount);
    void setPaddingBWCells(double amount);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);
    void setNumCellsHorizontally(int numCellsH);
    void setNumCellsVertically(int numCellsV);
    void setCellWidth(double width);
    void setCellHeight(double height);

public slots:
    void onItemAddedOrRemovedFromInventory(Item* item);

private:
    double border_;
    double paddingBWCells_;
    QColor backgroundColor_;
    QPixmap backgroundPixmap_;
    bool backgroundIsPixmap_;
    int numCellsHorizontally_;
    int numCellsVertically_;
    double cellWidth_;
    double cellHeight_;

    QGraphicsPixmapItem* pixmapItem_;
    Inventory* inventory_;
    std::vector<InventoryCell*> cells_;
    Game* game_;

    // helper functions
    void draw_();
};

#endif // INVENTORYVIEWER_H
