#pragma once

#include "Vendor.h"

#include "Gui.h"
#include "Ability.h"
#include "Panel.h"

class QGraphicsItem;
class QGraphicsPixmapItem;
class QPointF;

namespace qge{

class Panel;

/// Represents a Gui that visualizes and allows mouse interaction with an Ability.
/// When the Ability in the AbilityCell is clicked, AbilityCell will emit a signal.
/// @author Abdullah Aghazadah
class AbilityCell: public Gui
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
    std::unique_ptr<QGraphicsPixmapItem> picture_;
    std::unique_ptr<Panel> background_;
    QPointer<Ability> ability_;

    void draw_();
};

}
