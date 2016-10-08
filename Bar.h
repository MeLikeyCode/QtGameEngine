#ifndef BAR_H
#define BAR_H

#include <QObject>
#include "Gui.h"
#include <QColor>
#include <QPixmap>

class QGraphicsPixmapItem;

class Bar : public QObject, public Gui
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
    QGraphicsPixmapItem* backgroundPixmapItem_;
    QGraphicsPixmapItem* foregroundPixmapItem_;

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

#endif // BAR_H
