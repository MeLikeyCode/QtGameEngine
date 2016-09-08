#ifndef GUI_H
#define GUI_H

#include <QPointF>

class QGraphicsItem;
class Game;

/// Represents a GUI element in a Game. A Gui is placed relative to the
/// top left hand corner of the screen. To get/set the position of the
/// Gui use viewPos() and setViewPos() respectively.
class Gui
{
    friend class Game;
public:
    Gui(QGraphicsItem* graphicsItem);

    QPointF viewPos();
    void setViewPos(const QPointF& pos);

protected:
    QGraphicsItem* graphicsItem_;

    QPointF viewPos_;
};

#endif // GUI_H
