#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "Gui.h"
#include <QGraphicsPixmapItem>
#include <QColor>
#include <QPixmap>
#include <QObject>

class QGraphicsSceneMouseEvent;
class QGraphicsItem;

/// Represents a GUI that has a little dragable bar ( called a "foreground bar")
/// that you can drag from one end to the other on a "background bar").
/// The ScrollBar will emit a positionChanged event whenever the foreground bar
/// is dragged. The position can be from 0 to 1, 0 being in the beggining and 1
/// being all the way at the end.
/// @author Abdullah Aghazadah
/// @date 10-11-16
class ScrollBar : public QObject, public QGraphicsPixmapItem, public Gui
{
    Q_OBJECT
public:
    ScrollBar();

    QGraphicsItem* getGraphicsItem();

    // setters
    void setBgBarWidth(double width);
    void setBgBarLength(double length);
    void setFgBarWidth(double width);
    void setFgBarLength(double length);
    void setBgBarColor(const QColor& color);
    void setFgBarColor(const QColor& color);
    void setBgBarPixmap(const QPixmap& pixmap);
    void setFgBarPixmap(const QPixmap& pixmap);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

signals:
    /// Emitted when the position of the dragable bar changes.
    /// The position is from 0-1, 0 being at the very beginning and 1 being at the very end.
    /// The position is the position of the *center* of the dragable bar.
    void positionChanged(double newPosition);

private:
    double bgBarWidth_;
    double bgBarLength_;
    double fgBarWidth_;
    double fgBarLength_;
    bool bgBarIsColor_;
    bool fgBarIsColor_;
    QColor bgBarColor_;
    QColor fgBarColor_;
    QPixmap bgBarPixmap_;
    QPixmap fgBarPixmap_;

    double fgBarPosition_;

    bool fgBarIsBeingDragged_;

    QGraphicsPixmapItem* fgBar_;

    void draw_();
};

#endif // SCROLLBAR_H
