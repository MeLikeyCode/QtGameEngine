#pragma once

#include "Vendor.h"

#include "Gui.h"

class QGraphicsPixmapItem;

namespace qge{

/// A Gui that represents a progress bar of some sorts.
/// @author Abdullah Aghazadah
class Bar: public Gui
{
    Q_OBJECT
public:
    Bar();

    QGraphicsItem* getGraphicsItem();

    void setMinValue(double minValue);
    double minValue();
    void setMaxValue(double maxValue);
    double maxValue();
    void setCurrentValue(double currentValue);
    double currentValue();

    void setBackgroundColor(const QColor& color);
    void setForegroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);
    void setForegroundPixmap(const QPixmap& pixmap);

    void setWidth(double width);
    void setHeight(double height);
    void setVerticalPadding(double verticalPadding);
    void setHorizontalPadding(double horizontalPadding);

private:
    std::unique_ptr<QGraphicsPixmapItem> backgroundPixmapItem_;
    std::unique_ptr<QGraphicsPixmapItem> foregroundPixmapItem_;

    double minValue_;
    double maxValue_;
    double currentValue_;
    bool valuesAreInvalid_;

    double width_;
    double height_;
    double verticalPadding_;
    double horizontalPadding_;

    QColor backgroundColor_;
    QColor foregroundColor_;
    QPixmap backgroundPixmap_;
    QPixmap foregroundPixmap_;
    bool backgroundIsPixmap_;
    bool foregroundIsPixmap_;

    void checkValues_();
    void draw_();
};

}
