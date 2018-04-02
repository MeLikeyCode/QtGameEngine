#include "Quest.h"

using namespace qge;

/// Constructs a Quest with the specified name, description and optionally, initial status.
Quest::Quest(const std::string &name, const std::string &description, QuestStatus initialStatus):
    name_(name),
    description_(description),
    status_(initialStatus)
{

}

/// Sets the Status of the Quest.
void Quest::setStatus(QuestStatus newStatus)
{
    bool statusChanged = newStatus != status_;

    status_ = newStatus;

    if (statusChanged)
        emit questStatusChanged(this, newStatus);
}

/// Returns the current status of the Quest.
QuestStatus Quest::status()
{
    return status_;
}

/// Returns the name of the Quest.
std::string Quest::name()
{
    return name_;
}

std::string Quest::description()
{
    return description_;
}
