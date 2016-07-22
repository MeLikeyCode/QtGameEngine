#ifndef BUTTON_H
#define BUTTON_H

#include "Gui.h"
#include <string>
#include <QGraphicsRectItem>
#include <QColor>

class QGraphicsTextItem;

/// Represents a button Gui that can be added to a Game. You can set various
/// options such as the text color, padding b/w the text and the button's edges,
/// weather the text is bold/italic, etc...
/// When the Button is clicked, it will emit a clicked() signal.
class Button : public QObject, public Gui, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Button();

    void setBorderPadding(double amount);
    void setFontSize(int size);
    void setText(const std::string& text);
    void setTextColor(const QColor& color);
    void setTextBold(bool tf);
    void setTextItalic(bool tf);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    /// Emitted when the Button is clicked.
    void clicked();
private:
    QGraphicsTextItem* textItem_;
    double borderPadding_;
    QColor textColor_;
    bool textIsBold_;
    bool textIsItalic_;
    int fontSize_;
    std::string text_;

    // helper functions
    void redraw_();
};

#endif // BUTTON_H
