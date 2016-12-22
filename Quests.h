#ifndef QUESTS_H
#define QUESTS_H

#include <QObject>
#include <vector>
#include "Quest.h"

/// Represents a collection of Quests. Will emit events whenever a quest
/// is added, removed, or if the status of any quest is changed.
class Quests : public QObject
{
    Q_OBJECT
public:
    Quests();

    void addQuest(Quest* quest);
    void removeQuest(Quest* quest);
    int numOfQuests();

    Quest* quest(int i);

signals:
    /// Emitted whenever a Quest is added to the Quests.
    void questAdded(Quest* newQuest);

    /// Emitted whenever a Quest is removed from the Quests.
    /// This can happen via removeQuest() or if one the quest gets
    /// deconstructed.
    void questRemoved(Quest* removedQuest);

    /// Emitted whenever the status of one of the quests in the collection
    /// changes.
    void questStatusChanged(Quest* forQuest, QuestStatus newStatus);

public slots:
    void emitQuestStatusChanged_(Quest *quest, QuestStatus newStatus);
    void onQuestDestructed(QObject* quest);

private:
    std::vector<Quest*> quests_;
};

#endif // QUESTS_H
