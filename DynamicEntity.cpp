#include "DynamicEntity.h"
#include <string>
#include <cassert>
#include "Map.h"
#include "Sprite.h"
#include <QDebug> // TODO: remove, test
#include <QGraphicsScene>
#include "CountExpiringTimer.h"
#include <QLineF>
#include "Game.h"
#include "MoveRelativeToScreen.h"
#include <QGraphicsLineItem> // TODO: remove
#include "Inventory.h"
#include "EquipableItem.h"
#include "Slot.h"
#include "AsyncShortestPathFinder.h"
#include "Node.h"

/// Default constructor.
DynamicEntity::DynamicEntity():
    stepSize_(5),
    stepFrequency_(50),
    targetPointIndex_(0),
    rotationFrequency_(3),
    targetAngle_(0)
{
    // constructor body
    // = some defaults=
    moveTimer_ = new QTimer(this);
    rotationTimer_ = new QTimer(this);
    isPlayerControlled_ = false;
    moveBehavior_ = new MoveRealtiveToScreen();
    moveBehavior_->setEntity(this);

    fieldOfViewAngle_ = 90;
    fieldOfViewDistance_ = 500;

    inventory_ = new Inventory();
    inventory_->entity_ = this;

    timeStuck_ = 0;

    pf_ = new AsyncShortestPathFinder();
    connect(pf_,SIGNAL(pathFound(std::vector<QPointF>)),this,SLOT(followPath_(std::vector<QPointF>)));

    // listen to events
    connect(this,&DynamicEntity::collided,this,&DynamicEntity::onCollided);
}

DynamicEntity::~DynamicEntity()
{
    delete pf_;
    delete inventory_;
}

/// Add the specified Slot to the DynamicEntity.
void DynamicEntity::addSlot(Slot *slot)
{
    stringToSlot_[slot->name()] = slot;
    slot->owner_ = this;
}

/// Returns the Slot with the specified name.
/// Returns null if no such slot exists.
Slot *DynamicEntity::slot(std::string name)
{
    if (stringToSlot_.count(name) == 0){
        return nullptr;
    }

    return stringToSlot_[name];
}

/// Returns all the slots the Entity has.
std::unordered_set<Slot *> DynamicEntity::getSlots()
{
    std::unordered_set<Slot*> allSlots;
    for (std::pair<std::string,Slot*> stringSlotPair:stringToSlot_){
        allSlots.insert(stringSlotPair.second);
    }
    return allSlots;
}


/// Attempts to equip the specified item at the specified Slot.
/// Can only equip Items that are in the DynamicEntity's Inventory.
void DynamicEntity::equipItem(EquipableItem *item, std::string slot)
{
    // make sure Entity has the specified Slot
    Slot* slotToEquipIn = this->slot(slot);
    assert(slotToEquipIn != nullptr);

    // equip the Item into the Slot
    slotToEquipIn->equip(item);
}

/// Overload of DyanmicEntity::equipItem(EquipableItem*,std::string).
void DynamicEntity::equipItem(EquipableItem *item, Slot *slot)
{
    equipItem(item,slot->name());
}

/// Adds the specified Item to the Inventory of the DynamicEntity.
void DynamicEntity::addItemToInventory(Item *item)
{
    inventory_->addItem(item);
}

/// Removes the specified Item from the Inventory of the DynamicEntity
void DynamicEntity::removeItemFromInventory(Item *item)
{
    inventory_->removeItem(item);
}

/// Returns true if the Inventory of the DynamicEntity contains the specified
/// Item.
bool DynamicEntity::inventoryContains(Item *item)
{
    return inventory_->contains(item);
}

/// Returns the inventory of the Entity.
Inventory *DynamicEntity::inventory()
{
    return inventory_;
}

/// Causes the Entity to take 1 step closer to moving to its target point.
void DynamicEntity::moveStepAIControlled_(){
    // if there are no points to follow, don't do anything
    if (pointsToFollow_.size() == 0){
        stopAutomaticMovement();
        return;
    }

    // if there are no more points to follow and entity has reached its target
    // - stop moving
    if (targetPointIndex_ >= pointsToFollow_.size() - 1 && targetPointReached()){
        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        setCellPos(map()->pathingMap().pointToCell(snapPt));

        stopAutomaticMovement();

        return;
    }

    // if the entity has been stuck for long enough, recalculate
    if (timeStuck_ > 1500){
        // recalculate path (in case pathingmap changed)
        moveTo(pointsToFollow_.back());

        timeStuck_ = 0;
    }

    // if there are more points to follow and entity has reached its target
    // - snap to current target
    // - set next point as target
    // - face that point
    if (targetPointIndex_ < pointsToFollow_.size() - 1 && targetPointReached()){
        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        setCellPos(map()->pathingMap().pointToCell(snapPt));

        // set next point as target
        ++targetPointIndex_;

        // face target
        rotateTo(pointsToFollow_[targetPointIndex_]);
    }

    // take a step closer towards the target
    stepTowardsTarget();

    // check collisions and emit
    QRectF entityRect = sprite()->boundingRect();
    entityRect.moveTopLeft(pointPos());
    std::unordered_set<Entity*> collidingEntities = map()->entities(entityRect);
    emit collided(collidingEntities);
}

/// Causes the Entity to take 1 step towards completing it's rotation.
void DynamicEntity::rotateStep()
{
    // if it has reached its targetAngle, stop rotating
    if (abs(facingAngle() - targetAngle_) == 0 ){
        rotationTimer_->disconnect();
    }
    // other wise, rotate once towards targetAngle
    else {
        rotateTowardsTargetAngle();
    }
}

/// Returns true if the Entity is close enough to its targetPoint_.
bool DynamicEntity::targetPointReached(){
    // get a line b/w entity's pos and the targetPos
    QLineF ln(pointPos(),pointsToFollow_[targetPointIndex_]);

    // if the length of this line is less than a step size, return true
    return ln.length() < stepSize_;
}

/// Causes the DynamicEntity to attempt to takes a step towards the targetPoint.
/// If the Entity cannot take a step, timeStuck_ will be incremented.
void DynamicEntity::stepTowardsTarget(){
    // get a line b/w the entity's pos and the target pos
    QLineF ln(pointPos(),pointsToFollow_[targetPointIndex_]);

    // set the length of this line to be the same as stepSize
    ln.setLength(stepSize_);

    // find new pos
    double newX = pointPos().x() + ln.dx();
    double newY = pointPos().y() + ln.dy();
    QPointF newPt(newX,newY);

//    // move if the new pos is free, otherwise increment timeStuck_
//    if (canFit(newPt)){
//        setPointPos(newPt);
//    } else{
//        timeStuck_ += stepFrequency_;
//    }
    setPointPos(newPt);

}

/// Rotates 1 degrees towards the target angle
void DynamicEntity::rotateTowardsTargetAngle()
{
    // rotate right if rotateRight
    if (rotateRight_){
        rotateRight();
    }

    // other wise rotate left
    else {
        rotateLeft();
    }
}

void DynamicEntity::setStepSize(int stepSize){
    stepSize_ = stepSize;
}

int DynamicEntity::stepSize(){
    return stepSize_;
}

void DynamicEntity::setStepFrequency(int to){
    stepFrequency_ = to;
}

int DynamicEntity::stepFrequency(){
    return stepFrequency_;
}

int DynamicEntity::rotationFrequency() const
{
    return rotationFrequency_;
}

void DynamicEntity::setRotationFrequency(int freq)
{
    rotationFrequency_ = freq;
}

/// Rotates the Entity 1 degrees clockwise.
void DynamicEntity::rotateRight()
{
    sprite()->setRotation(sprite()->rotation()+1);
}

/// Rotates the Entity 1 degrees counter clockwise.
void DynamicEntity::rotateLeft()
{
    sprite()->setRotation(sprite()->rotation()-1);
}

/// Rotates the Entity clockwise by the specified number of degrees.
void DynamicEntity::rotateRight(int degrees)
{
    // stop previous rotations
    stopRotating();

    // set targetAngle and direction
    targetAngle_ = facingAngle() + degrees;
    rotateRight_ = true;

    // start the timer
    connect(rotationTimer_,SIGNAL(timeout()),this,SLOT(rotateStep()));
    rotationTimer_->start(rotationFrequency());

}

/// @see Entity:rotateRight(int)
void DynamicEntity::rotateLeft(int degrees)
{
    // stop previous rotations
    stopRotating();

    // set targetAngle and direction
    targetAngle_ = facingAngle() - degrees;
    rotateRight_ = false;

    // start the timer
    connect(rotationTimer_,SIGNAL(timeout()),this,SLOT(rotateStep()));
    rotationTimer_->start(rotationFrequency_);
}

/// Rotates the Entity until it faces the specified angle. The Entity will
/// rotate in the direction that is the fastest to the specified angle. A
/// specified angle of 0 degrees is right, 90 degrees is down and so on. The
/// specified angle must be between 0-360 degrees.
void DynamicEntity::rotateTo(int angle)
{
    // make sure angle is between 0-360
    assert(angle >= 0);
    assert(angle <= 360);

    QLineF line(QPointF(0,0),QPointF(1,1));
    line.setAngle(-facingAngle());

    QLineF line2(QPointF(0,0),QPointF(1,1));
    line2.setAngle(-angle);

    double angleBWLines = line.angleTo(line2);

    if (angleBWLines < 180){
        // rotate left
        rotateLeft(angleBWLines);
    }
    else {
        // rotate right
        rotateRight(360-angleBWLines);
    }


}

/// Rotates the Entity until it faces the specified point. The Entity will
/// rotate in whatever direction is the fastest.
void DynamicEntity::rotateTo(QPointF point)
{
    QLineF line(pointPos(),point);
    int r = 360-line.angle();
    rotateTo(r);
}

/// Stops the Entity's rotating.
void DynamicEntity::stopRotating()
{
    rotationTimer_->disconnect();
}

/// Makes the DynamicEntity follow the specified list of points ("path").
void DynamicEntity::followPath_(std::vector<QPointF> path){
    // stop following previous list of points (if any)
    stopAutomaticMovement();

    // follow this list of pts
    pointsToFollow_ = path;
    targetPointIndex_ = 0;
    connect(moveTimer_,SIGNAL(timeout()),this,SLOT(moveStepAIControlled_()));
    moveTimer_->start(stepFrequency());

    // play walk animation
    sprite()->play("walk",-1,100);
}

/// Executed in response to the the DynamicEntity colliding with something.
void DynamicEntity::onCollided(std::unordered_set<Entity *> entities)
{
    for (Entity* entity:entities){
        // item that is on the ground
        Item* asItem = dynamic_cast<Item*>(entity);
        if (asItem){
            if (asItem->inventory() == nullptr){
                addItemToInventory(asItem);
            }
        }
    }
}

/// Tells the Entity to move to the specified position.
///
/// Please ensure that the Entity has a "walk" animation.
void DynamicEntity::moveTo(QPointF pos){
    // temporarly disable entities own footing so a path can be retrieved
    // disablePathingMap();

    // get list of points from map (in a diff thread)
    pf_->findPath(map()->pathingMap(),pointPos(),pos);
}

/// Tells the Entity to move to the specified cell.
///
/// @see Entity::moveTo(const QPointF&)
void DynamicEntity::moveTo(const Node &cell)
{
    moveTo(map()->cellToPoint(cell));
}

/// Tells the Entity to move up by one cell of the Map's PathingMap.
void DynamicEntity::moveUp(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setY(c.y()-1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to move up down one cell of the Map's PathingMap.
void DynamicEntity::moveDown(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setY(c.y()+1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to move left by one cell of the Map's PathingMap.
void DynamicEntity::moveLeft(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setX(c.x()-1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to move right by one cell of the Map's PathingMap.
void DynamicEntity::moveRight(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setX(c.x()+1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to stop moving.
void DynamicEntity::stopAutomaticMovement(){
    moveTimer_->disconnect();
    pointsToFollow_.clear();
    targetPointIndex_ = 0;

    // play stand animation
    sprite()->play("stand",-1,100);
}

/// Sets the movement behavior of the Entity when being controlled by a player.
void DynamicEntity::setPlayerControlledMoveBehavior(PlayerControlledMoveBehavior *behavior)
{
    moveBehavior_ = behavior;
}

/// Returns true if the DynamicEntity is currently moving.
bool DynamicEntity::isMoving()
{
    return pointsToFollow_.size() > 0;
}

/// Returns true if the Entity is controlled by a player.
bool DynamicEntity::isPlayerControlled()
{
    return isPlayerControlled_;
}

/// Sets whether the Entity is controlled by the player or not.
///
/// If the Entity is controlled by a player, it will move in response to
/// keyboard/mouse input. If the Entity is not controlled by a player, it will
/// move in response to its own thinking.
void DynamicEntity::setPlayerControlled(bool tf)
{
    isPlayerControlled_ = tf;

    // if player controlled
    if (tf == true){
        // connect moveTimer_ with movePlayerControlled()
        moveTimer_->disconnect();
        connect(moveTimer_,SIGNAL(timeout()),this,SLOT(moveStepPlayerControlled()));
        moveTimer_->start(stepFrequency());
    }
    else{
        // connect moveTimer_ with moveAIControlled()
        moveTimer_->disconnect();
    }
}

/// Returns the Entities that are in the field of view of this Entity.
std::unordered_set<Entity *> DynamicEntity::entitiesInView()
{
    // - create QPolygon triangel w/ distance and angle
    // - pass this triangle to map to get entities in their

    QPointF p1(mapToMap(QPointF(0,0)));
    QLineF adjacent(p1,QPointF(-5,-5));
    adjacent.setAngle(-1 * this->facingAngle());
    adjacent.setLength(this->fieldOfViewDistance_);
    QLineF topL(adjacent);
    topL.setAngle(topL.angle() + this->fieldOfViewAngle_/2);
    QPointF p2(topL.p2());
    QLineF bottomL(adjacent);
    bottomL.setAngle(bottomL.angle() - this->fieldOfViewAngle_/2);
    QPointF p3(bottomL.p2());

    QVector<QPointF> points;
    points.append(p1);
    points.append(p2);
    points.append(p3);

    QPolygonF poly(points);

    std::unordered_set<Entity*> entities = map()->entities(poly);
    entities.erase(this);

    return entities;

//    // visualize line of site
//    QGraphicsLineItem* line = new QGraphicsLineItem();
//    line->setLine(topL);
//    QGraphicsLineItem* line2 = new QGraphicsLineItem();
//    line2->setLine(bottomL);
//    QGraphicsLineItem* line3 = new QGraphicsLineItem();
//    line3->setLine(QLineF(p2,p3));
//    map()->scene()->addItem(line);
//    map()->scene()->addItem(line2);
//    map()->scene()->addItem(line3);
}


/// Moves the Entity 1 step closer to its movement in response to keys.
void DynamicEntity::moveStepPlayerControlled()
{
    // delegate to moveBehavior
    moveBehavior_->moveStep();

    // check collisions and emit
    QRectF entityRect = sprite()->boundingRect();
    entityRect.moveTopLeft(pointPos());
    std::unordered_set<Entity*> collidingEntities = map()->entities(entityRect);
    emit collided(collidingEntities);
}
