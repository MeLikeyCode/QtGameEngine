#include "Quests.h"

using namespace qge;

Quests::Quests()
{

}

/// Adds the specified Quest.
/// Does nothing if the Quest has already been added.
void Quests::addQuest(Quest *quest)
{
    assert(quest != nullptr);

    // add the quest
    quests_.push_back(quest);

    // listen to when the status of the added quest changes
    connect(quest,&Quest::questStatusChanged,this,&Quests::emitQuestStatusChanged_);

    // listen to when the quest is destructed
    connect(quest, &Quest::destroyed, this, &Quests::onQuestDestructed_);

    emit questAdded(quest);
}

/// Removes the specified Quest.
/// Does nothing if the specified Quest does not exist.
void Quests::removeQuest(Quest *quest)
{
    assert(quest != nullptr);

    std::vector<Quest*>::iterator position = std::find(quests_.begin(), quests_.end(), quest);
    if (position != quests_.end()){
        quests_.erase(position);

        // stop listening to quest events
        disconnect(quest,&Quest::questStatusChanged,this,&Quests::emitQuestStatusChanged_);
        disconnect(quest, &Quest::destroyed, this, &Quests::onQuestDestructed_);

        emit questRemoved(quest);
    }
}

/// Returns the number of Quest objects in this Quests.
int Quests::numOfQuests()
{
    return quests_.size();
}

/// Returns the ith quest.
Quest *Quests::quest(int i)
{
    return quests_[i];
}

/// Emits that a Quest's status has been changed.
void Quests::emitQuestStatusChanged_(Quest* quest, QuestStatus newStatus)
{
    emit questStatusChanged(quest, newStatus);
}

/// Executed when one of the quests in the collection has been deconstructd.
/// Will remove it and emit removed signal.
void Quests::onQuestDestructed_(QObject *quest)
{
    Quest* asQuest = dynamic_cast<Quest*>(quest);
    assert(asQuest);

    removeQuest(asQuest);
}
