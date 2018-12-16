#pragma once

#include "Vendor.h"

#include "ScrollBar.h"
#include "Gui.h"

namespace qge{

class Panel;

/// Represents a Gui that can be used to visualized other Guis in a scroll like
/// fashion. You can add Gui's to this Gui and then use the scroll bars to
/// view the different Gui's as you scroll...I hope that makes sense :),
/// if not, try it out :)
/// @author Abdullah Aghazadah
/// @date 10-9-16
class ScrollWindow : public Gui
{
    Q_OBJECT
public:
    ScrollWindow();
    ScrollWindow(double width, double height);

    // options
    void setHeight(double height);
    void setWidth(double width);
    double height();
    double width();
    void showBorder(bool tf);
    void setBorderColor(const QColor& color);
    void setBorderThickness(double thickness);
    void showBackground(bool tf);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);

    // add/remove/check for gui containment
    void add(Gui* gui, QPointF atPos);
    void remove(Gui* gui);
    void removeAll();
    bool contains(Gui* gui);
    std::unordered_set<Gui*> addedGuis();


    QGraphicsItem* getGraphicsItem();

public slots:
    void verticalOrHorizontalScrollBarPositionChanged_(double pos);

private:
    double width_;
    double height_;

    std::unordered_map<Gui*,QPointF> guiToPos_; // the guis added, and the position added to
    std::unordered_map<Gui*,QPointF> guiInViewToShiftVector_; // the guis in viewport and their shift vectors

    std::unique_ptr<ScrollBar> verticalScrollBar_;
    ScrollBar* horizontalScrollBar_;
    Panel* background_;

    void draw_();
};

}
