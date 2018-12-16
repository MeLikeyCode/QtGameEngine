#pragma once

#include "Vendor.h"

#include "Gui.h"

namespace qge{

/// Represents a label Gui that can be added to a Game.
/// You can set various options such as the font, font size, font color, and width.
/// Oh and you can set the actual text, of course.
/// The Label emits a signal when the mouse hovers over it, when it clicks it, and
/// when it stops hovering over it.
class Label : public Gui, public QGraphicsItem
{
    Q_OBJECT
public:
    Label(const std::string& text="");

    QGraphicsItem* getGraphicsItem();

    void setFontSize(int fontSize);
    void setFontColor(const QColor& fontColor);
    void setWidth(double width);
    void setText(const std::string& text);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    /// Emitted when the mouse is hovered over the Label.
    void hovered();

    /// Emitted when the Label is clicked.
    void clicked(Label* label);

    /// Emitted when the mouse stops hovering over the Label (i.e. it "leaves" it).
    void unhovered();
private:
    QGraphicsTextItem* textItem_;
    std::string font_;
    int fontSize_;
    QColor fontColor_;
    double width_;
    std::string text_;

    void draw_();

    // QGraphicsItem interface
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

}
