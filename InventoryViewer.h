#ifndef INVENTORYVIEWER_H
#define INVENTORYVIEWER_H

#include "QGraphicsRectItem"
#include <QObject>
#include "Gui.h"

class Inventory;
class QPointF;
class InventoryCell;
class Item;
class Game;

/// Represents a GUI element that visualizes and allows interaction with
/// an Inventory.
class InventoryViewer : public QObject, public Gui
{
    Q_OBJECT
    // Game needs to access rectItem_ (rectItem_ needs to be added to Game's scene when
    // the InventoryViewer is added to the Game)
    friend class Game;
public:
    InventoryViewer(Game* game, Inventory* inventory=nullptr);

    void setInventory(Inventory* inventory);

    void setBorder(double amount);
    void setPaddingBWCells(double amount);
    void setBackgroundColor(const QColor& color);
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
    int numCellsHorizontally_;
    int numCellsVertically_;
    double cellWidth_;
    double cellHeight_;

    QGraphicsRectItem* rectItem_;
    Inventory* inventory_;
    std::vector<InventoryCell*> cells_;
    Game* game_;

    // helper functions
    void draw_();
};

#endif // INVENTORYVIEWER_H
