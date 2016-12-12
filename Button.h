#ifndef BUTTON_H
#define BUTTON_H

#include "Gui.h"
#include <string>
#include <QColor>

class QGraphicsTextItem;
class QGraphicsPixmapItem;

// TODO: reimplement using Panel as background (reduces duplicate code)

/// Represents a button Gui that can be added to a Game. You can set various
/// options such as the text color, padding b/w the text and the button's edges,
/// weather the text is bold/italic, etc...
/// When the Button is clicked, it will emit a clicked() signal.
class Button : public Gui
{
    Q_OBJECT
public:
    Button();

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
    QGraphicsPixmapItem* background_;
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

#endif // BUTTON_H
