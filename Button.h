#ifndef BUTTON_H
#define BUTTON_H

#include "Gui.h"
#include <string>
#include <QGraphicsRectItem>

class QGraphicsTextItem;

class Button : public QObject, public Gui, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Button();

    void setBorderPadding(double amount);
    void setFontSize(int size);
    void setText(const std::string& text);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    /// Emitted when the Button is clicked.
    void clicked();
private:
    QGraphicsTextItem* textItem_;
    double borderPadding_;
    int fontSize_;
    std::string text_;

    // helper functions
    void redraw_();
};

#endif // BUTTON_H
