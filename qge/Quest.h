#pragma once

#include "Vendor.h"

namespace qge{

/// Enumeration representing the status of a Quest.
enum class QuestStatus {
    IN_PROGRESS,
    COMPLETED,
    FAILED
};

/// Represents a quest, which has a name, description, and a current status such
/// as in progress, completed, or failed.
/// @author Abdullah Aghazadah
/// @date 8-21-16
class Quest : public QObject
{
    Q_OBJECT
public:
    Quest(const std::string& name, const std::string& description,
          QuestStatus initialStatus = QuestStatus::IN_PROGRESS);

    void setStatus(QuestStatus newStatus);

    QuestStatus status();
    std::string name();
    std::string description();

signals:
    /// Emitted whenver the status of the Quest *changes*.
    void questStatusChanged(Quest* quest, QuestStatus newStatus);

private:
    std::string name_;
    std::string description_;
    QuestStatus status_;
};

}
