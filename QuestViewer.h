#ifndef QUESTVIEWER_H
#define QUESTVIEWER_H

#include <QObject>
#include "Gui.h"
#include <unordered_map>

class Quests;
class Panel;
class Button;
class Label;
class Quest;

/// Gui that can visualize a bunch of quests (Quests class).
/// Click on a quest to view its description. Click close to close the Questviewer.
/// @author Abdullah Aghazadah
/// @date 9/5/16
class QuestViewer: public QObject, public Gui
{
    Q_OBJECT
public:
    QuestViewer(Quests* quests=nullptr);

    void setQuests(Quests* quests);

    QGraphicsItem* getGraphicsItem();

public slots:
    void draw_();
    void labelClicked_(Label* label);

private:
    Quests* quests_;

    Panel* outterPanel_;
    std::vector<Label*> questLabels_;
    Label* selectedQuestDescription_;
    Button* closeButton_;

    std::unordered_map<Label*,Quest*> labelToQuest_;
};

#endif // QUESTVIEWER_H
