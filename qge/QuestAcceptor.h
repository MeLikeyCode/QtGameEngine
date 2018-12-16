#pragma once

#include "Vendor.h"

#include "Gui.h"
#include "QuestViewer.h"

namespace qge{

class Button;
class Quest;
class Quests;

/// Represents a Gui that allows you to select/accept quests.
/// Similar to a QuestViewer, the user is allowed to browse through some quests,
/// however the QuestAccepter allows you to "accept" a quest by clicking its
/// "Accept Selected Quest" button. Each time a Quest is accepted, a signal is emitted
/// telling you which quest was just accepted.
/// QuestAcceptor also has a close button, which simply closes the Gui (i.e. removes
/// it from the game).
///
/// The recommended way to use the QuestAcceptor is:
/// - create some Quest objects, put them in a Quests object
/// - call QuestAcceptor::setQuests() passing in the quests object
/// - listen to the quest accept signals to be notified of which quests the user accepts
/// @author Abdullah Aghazadah
/// @date 11/9/16
class QuestAcceptor : public Gui
{
    Q_OBJECT
public:
    QuestAcceptor(Game* game);

    void setQuests(Quests* quests);

    // TODO: create delegetory veneer functions

    QGraphicsItem* getGraphicsItem();
public slots:
    void onAcceptButtonClicked_();
    void onCloseButtonClicked_();
signals:
    /// Emitted when a Quest has been accepted (i.e. the "Accept Selected Quest" button
    /// was clicked while a Quest was selected).
    void questAccepted(Quest* quest);

private:
    std::unique_ptr<QuestViewer> questViewer_;
    Button* closeButton_;
    Button* acceptButton_;
    Game* game_; // needed for close button to remove the GUI
};

}
