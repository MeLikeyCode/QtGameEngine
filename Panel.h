#ifndef PANEL_H
#define PANEL_H

#include "Gui.h"
#include <QColor>
#include <QPixmap>
#include <QObject>
#include <QGraphicsPixmapItem>

class QGraphicsRectItem;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;

/// Represents a panel GUI element that can be placed in a Game.
/// You can modify various options such as the size, color, background image, etc...
/// The panel will emit events when the mouse hovers/unhovers over it and when
/// the mouse clicks it.
class Panel : public QObject, public QGraphicsPixmapItem, public Gui
{
    Q_OBJECT
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

    // mouse event handlers
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

    QGraphicsItem* getGraphicsItem();
signals:
    void clicked(Panel* panel, QPointF pos, int button);
    void mouseHoverStarted(Panel* panel, QPointF pos);
    void mouseHoverEnded(Panel* panel, QPointF pos);
    void mouseHoverMoved(Panel* panel, QPointF pos);

private:
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
