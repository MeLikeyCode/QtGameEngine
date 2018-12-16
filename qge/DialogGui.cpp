#include "DialogGui.h"

#include "Label.h"
#include "ScrollWindow.h"

using namespace qge;

/// Constructs a default DialogGui, which shows no Response/Choices.
DialogGui::DialogGui():
    currentResponse_(nullptr),
    topScrollWindow_(new ScrollWindow()),
    bottomScrollWindow_(new ScrollWindow()),
    responseLabel_(new Label())
{
    // set parent/child relationship up
    bottomScrollWindow_->setParentGui(topScrollWindow_.get());
    responseLabel_->setParentGui(topScrollWindow_.get());

    // set positions //TODO: temp, move to draw function, and parametrize to allow
    // user customization such as width, offset, etc..
    bottomScrollWindow_->setGuiPos(QPointF(0,topScrollWindow_->height()));
    responseLabel_->setGuiPos(QPointF(60,35));

    // set default look
    topScrollWindow_->setBackgroundPixmap(QPixmap(":/resources/graphics/misc/paper.png"));
    bottomScrollWindow_->setBackgroundPixmap(QPixmap(":/resources/graphics/misc/paper.png"));
}

/// Sets the currently displayed Response of the DialogGui.
void DialogGui::setResponse(Response *response)
{
    // make sure response is not nullptr
    assert(response != nullptr);

    currentResponse_ = response;
    draw_();
}

/// Adds a Response to the DialogGui. Make sure to add some choices for this
/// response as well (via DialogGui::addChoice(Response* response, Choice* forChoice).
/// The choices of a response are shown when that response is being displayed.
void DialogGui::addResponse(Response *response)
{
    // make sure response is not nullptr
    assert(response != nullptr);

    responseToChoices_[response] = std::unordered_set<Choice*>();
}

/// Adds a Choice for the specified Response.
/// See DialogGui and DialogGui::addResponse(Response*) for more info.
void DialogGui::addChoice(Response *forResponse, Choice *choice)
{
    // make sure the response exists
    assert(responseToChoices_.count(forResponse) != 0);

    // make sure the choice is not null
    assert(choice != nullptr);

    responseToChoices_[forResponse].insert(choice);
    draw_();
}

/// Sets the Response that is shown when the specified Choice is clicked.
void DialogGui::setResponseForChoice(Response *response, Choice *forChoice)
{
    // make sure the response exists
    assert(responseToChoices_.count(response) != 0);

    choiceToResponse_[forChoice] = response;
    draw_();
}

QGraphicsItem *DialogGui::getGraphicsItem()
{
    return topScrollWindow_->getGraphicsItem();
}

/// Executed whenever the label of a choice has been clicked.
/// Will find the response for the choice and set it as the current response.
/// Will also emit a signal notifying any listeners that the choice has been clicked.
void DialogGui::choiceLabelClicked_(Label *label)
{
    // makes no sense if label is nullptr
    assert(label != nullptr);

    Choice* labelsChoice = labelToChoice_[label];
    Response* response = choiceToResponse_[labelsChoice];
    currentResponse_ = response;

    emit choiceClicked(this, labelsChoice);

    draw_();
}

/// Draws the DialogGui in its current state, displaying the current response, and
/// the available choices for that response.
void DialogGui::draw_()
{   
    if (currentResponse_ == nullptr)
        return;

    // remove previous choice's labels
    bottomScrollWindow_->removeAll();
    labelToChoice_.clear();

    // draw top scroll window and content
    responseLabel_->setText(currentResponse_->text());

    // draw bottom scroll window and content
    // - get all the choices for current response
    // - for each choice,
    //      -create a label, put it in scroll window
    //      -add label to labelToChoice map
    //      -connect to click event of label
    std::unordered_set<Choice*> choices = responseToChoices_[currentResponse_];
    int i = 0;
    for (Choice* choice:choices){
        Label* choicesLabel = new Label();
        choicesLabel->setText(choice->text());
        bottomScrollWindow_->add(choicesLabel,QPointF(40,i*25+25));
        i++;
        labelToChoice_[choicesLabel] = choice;
        connect(choicesLabel,&Label::clicked,this,&DialogGui::choiceLabelClicked_);
    }
}

/// Constructs a Choice with the specified text.
/// The text of the Choice is what is shown...obviously :)
Choice::Choice(const std::string &text)
{
    text_ = text;
}

/// Returns the text of the Choice.
std::string Choice::text()
{
    return text_;
}

/// Constructs a Response with the specified text.
/// The text of the Response is what is actually shown...obviously :)
Response::Response(const std::string &text)
{
    text_ = text;
}

/// Returns the text of the Response.
std::string Response::text()
{
    return text_;
}
