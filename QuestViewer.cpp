#include "QuestViewer.h"
#include "Quests.h"
#include "Panel.h"
#include <QGraphicsRectItem>
#include "Label.h"
#include "Button.h"
#include "Quest.h"
#include "ScrollWindow.h"

QuestViewer::QuestViewer(Quests *quests):
    quests_(quests),
    outterPanel_(new Panel()),
    selectedQuestDescription_(new Label()),
    closeButton_(new Button()),
    scrollWindow_(new ScrollWindow(100,100))
{   
    // set parents
    selectedQuestDescription_->setParentGui(outterPanel_);
    closeButton_->setParentGui(outterPanel_);
    scrollWindow_->setParentGui(outterPanel_);
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

QGraphicsItem *QuestViewer::getGraphicsItem()
{
    return outterPanel_->getGraphicsItem();
}

/// Executed whenever the QuestViewer's Quests changes (a quest added, removed,
/// or status changed). Will update the QuestViewer's visuals to reflect the change.
void QuestViewer::draw_()
{   
    // remove old quest labels
    for (Label* label:questLabels_){
        label->setParentGui(nullptr);
    }

    selectedQuestDescription_->setText("select a quest to see its description");
    if (quests_ != nullptr){
        // create a quest label for each quest in quests
        for (int i = 0, n = quests_->numOfQuests(); i < n; i++){
            Label* label = new Label();
            label->setParentGui(outterPanel_);

            label->setText(quests_->quest(i)->name());
            labelToQuest_[label] = quests_->quest(i);
            connect(label,&Label::clicked,this,&QuestViewer::labelClicked_);

            questLabels_.push_back(label);
        }

        // set height of outter panel based on height of scroll window
        const double PAD_SPACE = 10;
        const double DESCRIPTION_HEIGHT = 100;
        outterPanel_->setHeight(scrollWindow_->height() + PAD_SPACE + DESCRIPTION_HEIGHT);

        // set width of outer panel
        const double QUEST_VIEW_WIDTH = 400;
        outterPanel_->setWidth(400);

        // place each quest label in ScrollWindow
        for (int i = 0, n = questLabels_.size(); i < n; i++){
            Label* label = questLabels_[i];
            scrollWindow_->add(label);
        }

        // place quest description after scroll window
        selectedQuestDescription_->setGuiPos(QPointF(0,scrollWindow_->height() + PAD_SPACE));
        selectedQuestDescription_->setWidth(QUEST_VIEW_WIDTH);
    }

    // put close button on bottom of outter label
    closeButton_->setGuiPos(QPointF(200,outterPanel_->height()));
}

/// Executed whenever a quest label is clicked.
/// Will find corrisponding quest and set its text in description.
void QuestViewer::labelClicked_(Label *label)
{
    Quest* quest = labelToQuest_[label];
    selectedQuestDescription_->setText(quest->description());
}
