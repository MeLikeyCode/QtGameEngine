#ifndef LABEL_H
#define LABEL_H

#include <QObject>
#include "Gui.h"
#include <string>
#include <QColor>
#include <QGraphicsSimpleTextItem>

/// Represents a label Gui that can be added to a Game.
/// You can set various options such as the font, font size, font color, and width.
/// Oh and you can set the actual text, of course.
/// The Label emits a signal when the mouse hovers over it, when it clicks it, and
/// when it stops hovering over it.
class Label : public Gui, public QGraphicsSimpleTextItem
{
    Q_OBJECT
public:
    Label();

    void setFontFamily(const std::string& fontFamily);
    void setFontSize(int fontSize);
    void setFontColor(const QColor& fontColor);
    void setWidth(double width);
    void setText(const std::string& text);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    /// Emitted when the mouse is hovered over the Label.
    void hovered();

    /// Emitted when the Label is clicked.
    void clicked(Label* label);

    /// Emitted when the mouse stops hovering over the Label (i.e. it "leaves" it).
    void unhovered();
private:
    std::string font_;
    int fontSize_;
    QColor fontColor_;
    double width_;
    std::string text_;

    void draw_();
};

#endif // LABEL_H
