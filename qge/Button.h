#pragma once

#include "Vendor.h"

#include "Gui.h"

class QGraphicsTextItem;

namespace qge{

// TODO: reimplement using Panel as background (reduces duplicate code)

/// Represents a button Gui that can be added to a Game. You can set various
/// options such as the text color, padding b/w the text and the button's edges,
/// weather the text is bold/italic, etc...
/// When the Button is clicked, it will emit a clicked() signal.
class Button : public Gui, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Button();

    QGraphicsItem* getGraphicsItem();

    // setters
    void setBorderPadding(double amount);
    void setFontSize(int size);
    void setText(const std::string& text);
    void setTextColor(const QColor& color);
    void setTextBold(bool tf);
    void setTextItalic(bool tf);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);

    double height();
    double width();

    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    /// Emitted when the Button is clicked.
    void clicked();
private:
    QGraphicsTextItem* textItem_;
    double borderPadding_; // amount of padding b/w text and outter rect
    QColor textColor_;
    QColor backgroundColor_;
    QPixmap backgroundPixmap_;
    bool backgroundIsPixmap_;
    bool textIsBold_;
    bool textIsItalic_;
    int fontSize_;
    std::string text_;

    // helper functions
    void draw_();
};

}
