#include "QuestAcceptor.h"

#include "QuestViewer.h"
#include "Game.h"
#include "Button.h"

using namespace qge;

QuestAcceptor::QuestAcceptor(Game *game):
    questViewer_(new QuestViewer()),
    closeButton_(new Button()),
    acceptButton_(new Button()),
    game_(game)
{
    // set parent/child relationships
    closeButton_->setParentGui(questViewer_.get());
    acceptButton_->setParentGui(questViewer_.get());

    // set initial look
    closeButton_->setGuiPos(QPointF(0,360));
    acceptButton_->setGuiPos(QPointF(145,360));
    closeButton_->setText("Close");
    acceptButton_->setText("Accept Selected Quest");

    // listen to button
    connect(closeButton_,&Button::clicked,this,&QuestAcceptor::onCloseButtonClicked_);
    connect(acceptButton_,&Button::clicked,this, &QuestAcceptor::onAcceptButtonClicked_);
}

/// Sets the Quests that the QuestAcceptor should show.
void QuestAcceptor::setQuests(Quests *quests)
{
    questViewer_->setQuests(quests);
}

QGraphicsItem *QuestAcceptor::getGraphicsItem()
{
    return questViewer_->getGraphicsItem();
}

/// Executed when the "Accept Selected Quest" button has been clicked.
/// Will emit a questAccepted signal.
void QuestAcceptor::onAcceptButtonClicked_()
{
    Quest* selectedQuest = questViewer_->selectedQuest();
    if (selectedQuest != nullptr)
        emit questAccepted(selectedQuest);
}

/// Executed when the "Close" button has been clicked.
/// Will close the QuestAcceptor (i.e. remove the Gui from the Game).
void QuestAcceptor::onCloseButtonClicked_()
{
    game_->removeGui(this);
}
