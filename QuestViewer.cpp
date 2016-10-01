#include "QuestViewer.h"
#include "Quests.h"
#include "Panel.h"

QuestViewer::QuestViewer(Quests *quests):
    Gui(nullptr),
    quests_(quests)
{
    questsUpdated_();
}

/// Sets the Quests that the QuestViewer should visualize.
void QuestViewer::setQuests(Quests *quests)
{
    // stop listening to old quests
    if (quests_)
        quests_->disconnect();

    // listen to new quests
    connect(quests,&Quests::questAdded,this,&QuestViewer::questsUpdated_);
    connect(quests,&Quests::questRemoved,this,&QuestViewer::questsUpdated_);
    connect(quests,&Quests::questStatusChanged,this,&QuestViewer::questsUpdated_);

    questsUpdated_();
}

/// Executed whenever the QuestViewer's Quests changes (a quest added, removed,
/// or status changed). Will update the QuestViewer's visuals to reflect the change.
void QuestViewer::questsUpdated_()
{
    // set size of outter panel
}
