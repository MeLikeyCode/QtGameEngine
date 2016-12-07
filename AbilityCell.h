#ifndef ABILITYCELL_H
#define ABILITYCELL_H

#include <QObject>
#include "Gui.h"

class Ability;
class QGraphicsItem;
class QGraphicsPixmapItem;
class Panel;
class QPointF;

/// Represents a Gui that visualizes and allows mouse interaction with an Ability.
/// When the Ability in the AbilityCell is clicked, AbilityCell will emit a signal.
/// You can set the Ability of the AbilityCell by passing it into the constructor or
/// via its setter. There are several member functions provided which allow you to
/// modify the look/size of the AbilityCell.
class AbilityCell: public QObject, public Gui
{
    Q_OBJECT
public:
    AbilityCell(int width, int height, Ability* ability = nullptr);

    void setAbility(Ability* ability);
    Ability* ability();

    void setSize(int width, int height);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);

    QGraphicsItem* getGraphicsItem();

public slots:
    void onClicked_(Panel* panel, QPointF pos, int button);

signals:
    void clicked(AbilityCell* abilityCell, int button);

private:
    QGraphicsPixmapItem* picture_;
    Panel* background_;
    Ability* ability_;

    void draw_();
};

#endif // ABILITYCELL_H
