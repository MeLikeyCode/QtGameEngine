#ifndef PANEL_H
#define PANEL_H

#include "Gui.h"
#include <QColor>
#include <QPixmap>

class QGraphicsPixmapItem;

/// Represents a panel GUI element that can be placed in a Game.
/// You can modify various options such as the size, color, background image, etc...
class Panel : public Gui
{
public:
    Panel();

    QGraphicsItem* getGraphicsItem();

    void setColor(const QColor& color);
    void setPixmap(const QPixmap& pixmap);
    void setWidth(double width);
    void setHeight(double height);
private:
    QGraphicsPixmapItem* pixmapItem_;
    QPixmap pixmap_;
    QColor color_;
    bool isPixmap_;
    double width_;
    double height_;

    void draw_();
};

#endif // PANEL_H
