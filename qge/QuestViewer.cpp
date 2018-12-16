#include "QuestViewer.h"

#include "Quests.h"
#include "Panel.h"
#include "Label.h"
#include "Button.h"
#include "Quest.h"
#include "ScrollWindow.h"

using namespace qge;

QuestViewer::QuestViewer(Quests *quests):
    quests_(quests),
    selectedQuest_(nullptr),
    questLabels_(),
    labelToQuest_(),
    width_(360),
    height_(360),
    swWidth_(340),
    swHeight_(170),
    swX_(20),
    swY_(20),
    descWidth_(320),
    descLabelX_(30),
    descOffsetY_(20),
    outterPanel_(new Panel()),
    scrollWindow_(new ScrollWindow()),
    selectedQuestDescription_(new Label())
{   
    // set parents
    scrollWindow_->setParentGui(outterPanel_.get());
    selectedQuestDescription_->setParentGui(outterPanel_.get());

    // defaults
    outterPanel_->setBackgroundPixmap(QPixmap(":/resources/graphics/misc/paper.png"));
    scrollWindow_->showBackground(false);
    scrollWindow_->showBorder(false);
    selectedQuestDescription_->setText("Select a quest to see its description.");

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

    quests_ = quests;

    draw_();
}

/// Returns the currently selected quest in the quest viewer.
Quest *QuestViewer::selectedQuest()
{
    return selectedQuest_;
}

QGraphicsItem *QuestViewer::getGraphicsItem()
{
    return outterPanel_->getGraphicsItem();
}

/// Executed whenever the QuestViewer's Quests changes (a quest added, removed,
/// or status changed). Will update the QuestViewer's visuals to reflect the change.
void QuestViewer::draw_()
{   
    // remove old quest labels from the scroll window
    // (in case there are any new quests - we want to create labels for all quests
    // from scratch)
    for (Label* label:questLabels_){
        scrollWindow_->remove(label);
    }

    // size outter panel
    outterPanel_->setWidth(width_);
    outterPanel_->setHeight(height_);

    // position/size scroll window
    scrollWindow_->setGuiPos(QPointF(swX_,swY_));
    scrollWindow_->setWidth(swWidth_);
    scrollWindow_->setHeight(swHeight_);

    // position/size description label properly
    selectedQuestDescription_->setGuiPos(QPointF(descLabelX_,swY_+swHeight_+descOffsetY_));
    selectedQuestDescription_->setWidth(descWidth_);

    // create a quest label for each quest
    if (quests_ != nullptr){
        for (int i = 0, n = quests_->numOfQuests(); i < n; i++){
            Label* label = new Label();

            label->setText(quests_->quest(i)->name());
            labelToQuest_[label] = quests_->quest(i);
            connect(label,&Label::clicked,this,&QuestViewer::labelClicked_);

            questLabels_.push_back(label);

            qDebug() << "e";
        }
    }

    // place quest labels in scroll window
    for (int i = 0, n = questLabels_.size(); i < n; i++){
        Label* label = questLabels_[i];
        scrollWindow_->add(label,QPointF(25,i*40+25));
    }
}

/// Executed whenever a quest label is clicked.
/// Will find corrisponding quest and show its text in the description area.
/// Will also marked the quest as the currently selected one.
void QuestViewer::labelClicked_(Label *label)
{
    Quest* quest = labelToQuest_[label];
    selectedQuest_ = quest;
    selectedQuestDescription_->setText(quest->description());
}
