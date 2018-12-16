#pragma once

#include "Vendor.h"

#include "Gui.h"
#include "Panel.h"

namespace qge{

class Quest;
class Quests;
class Button;
class Label;
class ScrollWindow;

/// Gui that can visualize a bunch of quests (Quests class).
/// Click on a quest to view its description.
/// @author Abdullah Aghazadah
/// @date 9/5/16
class QuestViewer: public Gui
{
    Q_OBJECT
public:
    QuestViewer(Quests* quests=nullptr);

    void setQuests(Quests* quests);
    Quest* selectedQuest();

    QGraphicsItem* getGraphicsItem();

public slots:
    void draw_();
    void labelClicked_(Label* label);

private:
    Quests* quests_;
    Quest* selectedQuest_;
    std::vector<Label*> questLabels_;
    std::unordered_map<Label*,Quest*> labelToQuest_;

    double width_;      // width of the entire QuestViewer
    double height_;     // height " "
    double swWidth_;    // width of the ScrollWindow
    double swHeight_;   // height " "
    double swX_;        // x position of the scroll window relative to outterPanel_
    double swY_;        // y position " "
    double descWidth_;  // width of the description label
    double descLabelX_; // x position of the description label relative to outterPanel_
    double descOffsetY_;// distance between the bottom of the scrollWindow and the start of description label
    double closeBtnX_;  // the x position of the close button relative to outterPanel_
    double closeBtnY_;  // the y position of the close button relative to outterPanel_

    std::unique_ptr<Panel> outterPanel_;
    ScrollWindow* scrollWindow_;
    Label* selectedQuestDescription_;
};

}
