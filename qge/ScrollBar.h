#pragma once

#include "Vendor.h"

#include "Gui.h"

class QGraphicsSceneMouseEvent;
class QGraphicsItem;

namespace qge{

/// Represents a GUI that has a little dragable bar ( called a "foreground bar")
/// that you can drag from one end to the other on a "background bar".
/// The ScrollBar will emit a positionChanged event whenever the foreground bar
/// is dragged. The position can be from 0 to 1, 0 being in the beggining and 1
/// being all the way at the end.
///
/// Note: If the bar is full (i.e. length of background and foreground bars are the same),
/// the ScrollBar will not show up by default. To have it show up anyways,
/// call showEvenIfFull(true);
/// @author Abdullah Aghazadah
/// @date 10-11-16
class ScrollBar : public Gui, public QGraphicsPixmapItem
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
    void setFgBarLengthAsFractionOfBgBarLength(double fraction);
    void setBgBarColor(const QColor& color);
    void setFgBarColor(const QColor& color);
    void setBgBarPixmap(const QPixmap& pixmap);
    void setFgBarPixmap(const QPixmap& pixmap);
    void showEvenIfFull(bool tf);

    // getters
    double fgBarCenterPos();
    double fgBarTopPos();
    double fgBarBottomPos();
    double fgBarWidth();
    double fgBarLength();
    double bgBarWidth();
    double bgBarLength();

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
    bool showEvenIfFull_;

    double fgBarPosition_; // position of the center of the fgBar

    bool fgBarIsBeingDragged_;

    QGraphicsPixmapItem* fgBar_;

    void draw_();

    bool fgBarIsTooHigh_();
    bool fgBarIsTooLow_();
};

}
