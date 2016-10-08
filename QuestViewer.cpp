#include "QuestViewer.h"
#include "Quests.h"
#include "Panel.h"
#include <QGraphicsRectItem>

QuestViewer::QuestViewer(Quests *quests):
    Gui(nullptr),
    quests_(quests)
{
    graphicsItem_ = outterPanel_->graphicsItem_
    draw_();
}

/// Sets the Quests that the QuestViewer should visualize.
void QuestViewer::setQuests(Quests *quests)
{
    // stop listening to old quests
    if (quests_)
        quests_->disconnect();

    // listen to new quests
    connect(quests,&Quests::questAdded,this,&QuestViewer::draw_);
    connect(quests,&Quests::questRemoved,this,&QuestViewer::draw_);
    connect(quests,&Quests::questStatusChanged,this,&QuestViewer::draw_);

    draw_();
}

/// Executed whenever the QuestViewer's Quests changes (a quest added, removed,
/// or status changed). Will update the QuestViewer's visuals to reflect the change.
void QuestViewer::draw_()
{
    // set size of outter panel
}
