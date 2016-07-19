#ifndef BUTTON_H
#define BUTTON_H

#include "Gui.h"
#include <string>

class QGraphicsRectItem;
class QGraphicsTextItem;

class Button : public Gui
{
public:
    Button();

    void setBorderPadding(double amount);
    void setFontSize(int size);
    void setText(const std::string& text);

private:
    QGraphicsRectItem* backgroundRectItem_;
    QGraphicsTextItem* textItem_;
    double borderPadding_;
    int fontSize_;
    std::string text_;

    // helper functions
    void redraw_();
};

#endif // BUTTON_H
