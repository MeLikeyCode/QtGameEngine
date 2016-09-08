#include "Quests.h"
#include <cassert>

Quests::Quests()
{

}

/// Adds the specified Quest.
/// Does nothing if the Quest has already been added.
void Quests::addQuest(Quest *quest)
{
    assert(quest != nullptr);

    // add the quest
    quests_.insert(quest);

    // listen to when the status of the added quest changes
    connect(quest,&Quest::questStatusChanged,this,&Quests::emitQuestStatusChanged_);

    emit questAdded(quest);
}

/// Removes the specified Quest.
/// Does nothing if the specified Quest does not exist.
void Quests::removeQuest(Quest *quest)
{
    assert(quest != nullptr);

    quests_.erase(quest);
    emit questRemoved(quest);
}

/// Emits that a Quest's status has been changed.
void Quests::emitQuestStatusChanged_(Quest* quest, QuestStatus newStatus)
{
    emit questStatusChanged(quest, newStatus);
}
