#ifndef QUESTVIEWER_H
#define QUESTVIEWER_H

#include <QObject>
#include "Gui.h"

class Quests;
class Panel;
class Button;

/// Gui that can visualize a bunch of quests (Quests class).
/// Click on a quest to view its description. Click close to close the Questviewer.
/// @author Abdullah Aghazadah
/// @date 9/5/16
class QuestViewer: public QObject, public Gui
{
public:
    QuestViewer(Quests* quests=nullptr);

    void setQuests(Quests* quests);

public slots:
    void draw_();

private:
    Quests* quests_;

    double totalWidth_;
    double innerWidth_;
    double topHeight_;
    double bottomHeight_;

    Panel* outterPanel_;
};

#endif // QUESTVIEWER_H
