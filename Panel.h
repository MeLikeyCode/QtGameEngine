#ifndef PANEL_H
#define PANEL_H

#include "Gui.h"
#include <QColor>
#include <QPixmap>

class QGraphicsPixmapItem;
class QGraphicsRectItem;

/// Represents a panel GUI element that can be placed in a Game.
/// You can modify various options such as the size, color, background image, etc...
class Panel : public Gui
{
public:
    Panel();

    // setters
    void setWidth(double width);
    void setHeight(double height);
    void showBackground(bool tf);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);
    void showBorder(bool tf);
    void setBorderThickness(double thickness);
    void setBorderColor(const QColor& color);

    // getters
    int height();
    int width();

    QGraphicsItem* getGraphicsItem();
private:
    QGraphicsPixmapItem* background_; // background
    QGraphicsRectItem* border_; // border
    QPixmap backgroundPixmap_;
    QColor backgroundColor_;
    bool backgroundIsPixmap_;
    bool showBackground_;
    bool showBorder_;
    double borderThickness_;
    QColor borderColor_;
    double width_;
    double height_;


    void draw_();
};

#endif // PANEL_H
