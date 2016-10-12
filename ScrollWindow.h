#ifndef SCROLLWINDOW_H
#define SCROLLWINDOW_H

#include "Gui.h"
#include <QGraphicsPixmapItem>
#include <vector>

/// Represents a Gui that can be used to visualized other Guis in a scroll like
/// fashion. You can add Gui's to this Gui and then use the scroll bar to
/// view the different Gui's as you scroll...I hope that makes sense :),
/// if not, try it out :)
/// @author Abdullah Aghazadah
/// @date 10-9-16
class ScrollWindow : public QGraphicsPixmapItem, public Gui
{
public:
    ScrollWindow();
    ScrollWindow(double width, double height);

    void setHeight(double height);
    void setWidth(double width);
    double height();

    void add(Gui* gui);

    QGraphicsItem* getGraphicsItem();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    double width_;
    double height_;
    std::vector<Gui*> guis_;
    double scrollBarPosition_; // the position of the scrollbar from 0-1;
    bool scrollBarBeingDragged_;

    QGraphicsPixmapItem* scrollBarFG_; // foreground of scrollbar (thing you drag)

    void draw_();
};

#endif // SCROLLWINDOW_H
