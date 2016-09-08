#ifndef QUESTS_H
#define QUESTS_H

#include <QObject>
#include <unordered_set>
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

signals:
    void questAdded(Quest* newQuest);
    void questRemoved(Quest* removedQuest);
    void questStatusChanged(Quest* forQuest, QuestStatus newStatus);

public slots:
    void emitQuestStatusChanged_(Quest *quest, QuestStatus newStatus);

private:
    std::unordered_set<Quest*> quests_;
};

#endif // QUESTS_H
