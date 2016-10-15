
#ifndef SCROLLWINDOW_H
#define SCROLLWINDOW_H

#include "Gui.h"
#include <vector>
#include <QObject>
#include <QPointF>
#include <unordered_map>

class ScrollBar;

/// Represents a Gui that can be used to visualized other Guis in a scroll like
/// fashion. You can add Gui's to this Gui and then use the scroll bars to
/// view the different Gui's as you scroll...I hope that makes sense :),
/// if not, try it out :)
/// @author Abdullah Aghazadah
/// @date 10-9-16
class ScrollWindow : public QObject, public Gui
{
    Q_OBJECT
public:
    ScrollWindow();
    ScrollWindow(double width, double height);

    void setHeight(double height);
    void setWidth(double width);
    double height();
    double width();

    void add(Gui* gui, QPointF atPos);

    QGraphicsItem* getGraphicsItem();

public slots:
    void verticalOrHorizontalScrollBarPositionChanged_(double pos);

private:
    double width_;
    double height_;
    std::unordered_map<Gui*,QPointF> guiToPos_; // the guis added, and the position added to
    std::unordered_map<Gui*,QPointF> guiInViewToShiftVector_; // the guis in viewport and their shift vectors

    ScrollBar* verticalScrollBar_;
    ScrollBar* horizontalScrollBar_;

    void draw_();
};

#endif // SCROLLWINDOW_H
