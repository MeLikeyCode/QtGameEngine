#ifndef PANEL_H
#define PANEL_H

#include "Gui.h"
#include <QColor>

/// Represents a panel GUI element in a Game.
class Panel : public Gui
{
public:
    Panel();

    void setColor(const QColor& color);
    void setOpacity(double opacity);
private:

};

#endif // PANEL_H
