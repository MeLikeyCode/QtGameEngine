#ifndef GUI_H
#define GUI_H

#include <QPointF>
#include <QGraphicsItem>
#include <unordered_set>
#include <QObject>

class Game;
class QRectF;

/// Abstract class that Represents a GUI element in a Game. 
///
/// A Gui is placed relative to its parent Gui, if it does not have a parent
/// Gui, its place relative to the top left hand corner of the screen.
///
/// Most Guis offer functions for customizing their look in some way.
/// Most Guis emit signals when they are interacted with.
///@author Abdullah Aghazadah
class Gui: public QObject, public QGraphicsItem // inherits from QObject b/c most subclasses need to
                                                // inherits QGraphicsItem for parent/child positioning/deleting
{
    Q_OBJECT
public:
    Gui();

    QPointF guiPos();
    void setGuiPos(const QPointF& guiPos);
    void setGuiRotation(double degrees);

    void setParentGui(Gui* gui);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

protected:
    std::unordered_set<Gui*> children_;
    Gui* parent_;

    QPointF pos_;

    std::vector<QRectF> getBoundingBoxesFor_(QGraphicsItem* gi, QGraphicsItem *mapTo) const;
};

#endif // GUI_H
