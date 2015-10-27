#include "Entity.h"
#include <string>
#include <cassert>
#include "Map.h"
#include "Sprite.h"
#include <QDebug> // TODO: remove, test
#include <QGraphicsScene>
#include "CountExpiringTimer.h"
#include <QLineF>
#include "Game.h"

/// Default constructor.
Entity::Entity():
    pathingMap_(1,1,63), // default 1x1 filled (in body) PathingMap
    stepSize_(5),
    stepFrequency_(50),
    targetPointIndex_(0),
    rotationFrequency_(3),
    targetAngle_(0),
    canMoveWithKeys_(false),
    movingWithKeys_(false)
{
    // constructor body
    // = some defaults=
    map_ = nullptr;
    sprite_ = nullptr;
    automaticMoveTimer_ = new QTimer(this);
    rotationTimer_ = new QTimer(this);
    moveWithKeysTimer_ = new QTimer(this);

    // default sprite
    Sprite* spr = new Sprite();
    setSprite(spr);
}

Entity::~Entity()
{
    for (Entity* entity: children_){
        delete entity;
    }
}

/// Returns the PathingMap of the Entity.
PathingMap Entity::pathingMap() const{
    return pathingMap_;
}

void Entity::setPathingMap(const PathingMap &pathingMap){
    pathingMap_ = pathingMap;
}

/// Returns a pointer to the Map that the Entity is in.
Map *Entity::map() const{
    // make sure it has a map
    assert(map_);

    return map_;
}

void Entity::setMap(Map *toMap)
{
    map_ = toMap;
}

/// Returns the position of the Entity.
///
/// The position is returned relative to the parent Entity. If there is no
/// parent Entitiy, it is returned relative to the Map.
QPointF Entity::pointPos() const{
    return sprite()->pos();
}

/// Sets the position of the Entity.
///
/// The position is relative to the parent Entity. If there is no
/// parent Entitiy, it is relative to the Map.
void Entity::setPointPos(const QPointF &pos){
    sprite()->setPos(pos);
}

/// Returns the cell that the Entity is in.
Node Entity::cellPos(){
    return map()->pathingMap().pointToCell(pointPos());
}

/// Causes the Entity to take 1 step closer to moving to its target point.
void Entity::moveStepAutomatic(){
    // manual movment with keys should override this
    if (movingWithKeys_){
        stopAutomaticMovement();
    }

    // if there are no more points to follow and entity has reached its target
    // - stop moving
    if (targetPointIndex_ >= pointsToFollow_.size() - 1 && targetPointReached()){
        stopAutomaticMovement();

        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        setCellPos(map()->pathingMap().pointToCell(snapPt));

        return;
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

        // recalculate path (in case pathingmap changed) TODO enable
        moveTo(pointsToFollow_.back());
    }

    // take a step closer towards the target
    stepTowardsTarget();
}

/// Causes the Entity to take 1 step towards completing it's rotation.
void Entity::rotateStep()
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
bool Entity::targetPointReached(){
    // get a line b/w entity's pos and the targetPos
    QLineF ln(pointPos(),pointsToFollow_[targetPointIndex_]);

    // if the length of this line is less than a step size, return true
    return ln.length() < stepSize_;
}

/// Takes a step towards the targetPoint.
void Entity::stepTowardsTarget(){
    // get a line b/w the entity's pos and the target pos
    QLineF ln(pointPos(),pointsToFollow_[targetPointIndex_]);

    // set the length of this line to be the same as stepSize
    ln.setLength(stepSize_);

    // move by ln.dx and ln.dy
    double newX = pointPos().x() + ln.dx();
    double newY = pointPos().y() + ln.dy();
    QPointF newPt(newX,newY);
    setPointPos(newPt);
}

/// Rotates 1 degrees towards the target angle
void Entity::rotateTowardsTargetAngle()
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

/// Sets the Sprite of the Entity. Removes (but does not delete) the old Sprite.
void Entity::setSprite(Sprite *sprite){
    // set all children's sprite's parent to new sprite
    for (Entity* child: children()){
        child->sprite()->setParentItem(sprite);
    }

    // if the Entity is already in a map
    if (map_){
        // set the position of the new sprite to the pos of the old sprite
        sprite->setPos(sprite_->pos());

        // remove old sprite/add new sprite
        map()->scene()->removeItem(sprite_);
        map()->scene()->addItem(sprite);
    }

    // set internal sprite_ pointer to the new sprite
    sprite_ = sprite;

}

/// Returns the Entity's Sprite.
Sprite *Entity::sprite() const{
    // make sure the Entity has a sprite
    assert(sprite_);

    return sprite_;
}

QRectF Entity::boundingRect()
{
    return sprite()->boundingRect();
}

void Entity::setStepSize(int stepSize){
    stepSize_ = stepSize;
}

int Entity::stepSize(){
    return stepSize_;
}

void Entity::setStepFrequency(int to){
    stepFrequency_ = to;
}

int Entity::stepFrequency(){
    return stepFrequency_;
}

int Entity::rotationFrequency() const
{
    return rotationFrequency_;
}

void Entity::setRotationFrequency(int freq)
{
    rotationFrequency_ = freq;
}

/// Returns the current angle the Entity is facing in degrees.
int Entity::facingAngle()
{
    return sprite()->rotation();
}

/// Rotates the Entity 1 degrees clockwise.
void Entity::rotateRight()
{
    sprite()->setRotation(sprite()->rotation()+1);
}

/// Rotates the Entity 1 degrees counter clockwise.
void Entity::rotateLeft()
{
    sprite()->setRotation(sprite()->rotation()-1);
}

/// Rotates the Entity clockwise by the specified number of degrees.
void Entity::rotateRight(int degrees)
{
    // stop previous rotations
    stopRotating();

    // set targetAngle and direction
    targetAngle_ = facingAngle() + degrees;
    rotateRight_ = true;

    // start the timer
    connect(rotationTimer_,SIGNAL(timeout()),this,SLOT(rotateStep()));
    rotationTimer_->start(rotationFrequency_);

}

/// @see Entity:rotateRight(int)
void Entity::rotateLeft(int degrees)
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
void Entity::rotateTo(int angle)
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
void Entity::rotateTo(QPointF point)
{
    QLineF line(pointPos(),point);
    int r = 360-line.angle();
    rotateTo(r);
}

/// Stops the Entity's rotating.
void Entity::stopRotating()
{
    rotationTimer_->disconnect();
}

/// Instantly moves the Entity to the specified cell in the Map.
void Entity::setCellPos(const Node &cell){
    // make sure the entity has a Map
    assert(map_);

    // clear pathing map
    // disablePathingMap(); // only static entities have pathing maps

    // move to new region
    QPointF newPos = map()->pathingMap().cellToPoint(cell);
    setPointPos(newPos);

    // enable pathing map
    // enablePathingMap();
}

/// Tells the Entity to move to the specified position.
///
/// Please ensure that the Entity has a "walk" animation.
void Entity::moveTo(const QPointF &pos){
    // temporarly disable entities own footing so a path can be retrieved
    // disablePathingMap();

    // get list of points from map
    std::vector<QPointF> pts = map()->pathingMap().shortestPath(pointPos(),pos);

    // stop following previous list of points
    stopAutomaticMovement();

    // follow this list of pts
    pointsToFollow_ = pts;
    targetPointIndex_ = 1; // start at 1eth not 0eth point (to prevent
                           // back movement if two quick move commands are given)
    connect(automaticMoveTimer_,SIGNAL(timeout()),this,SLOT(moveStepAutomatic()));
    automaticMoveTimer_->start(stepFrequency());

    // play walk animation
    sprite()->play("walk",-1,100);
}

/// Tells the Entity to move up by one cell of the Map's PathingMap.
void Entity::moveUp(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setY(c.y()-1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to move up down one cell of the Map's PathingMap.
void Entity::moveDown(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setY(c.y()+1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to move left by one cell of the Map's PathingMap.
void Entity::moveLeft(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setX(c.x()-1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to move right by one cell of the Map's PathingMap.
void Entity::moveRight(){
    // get the location of one cell up
    Node c = map()->pathingMap().pointToCell(pointPos());
    c.setX(c.x()+1);
    QPointF pt = map()->pathingMap().cellToPoint(c);

    // move to this point
    moveTo(pt);
}

/// Tells the Entity to stop moving.
void Entity::stopAutomaticMovement(){
    automaticMoveTimer_->disconnect();

    // play stand animation
    sprite()->play("stand",-1,100);
}

/// Returns True if the Entity moves in response to key presses.
bool Entity::canMovesWithKeys()
{
    return canMoveWithKeys_;
}

/// Sets whether the Entity moves in response to key presses.
void Entity::setCanMoveWithKeys(bool b)
{
    canMoveWithKeys_ = b;

    if (b == true){
        connect(moveWithKeysTimer_,SIGNAL(timeout()),this,SLOT(moveStepWithKeys()));
        moveWithKeysTimer_->start(stepFrequency());
    }
    else{
        moveWithKeysTimer_->disconnect();
    }
}

/// Returns true if the Entity can fit at the specified point.
///
/// Takes the Entity's bounding rect and sees if it can fit in the specified point.
/// I.e if it doesn't intersect with any filled cells in the Map's PathingMap.
bool Entity::canFit(const QPointF &atPos)
{
    // get the bounding rect
    int CUSHION = 20;

    QRectF checkRegion = boundingRect();
    checkRegion.moveTo(atPos);
    checkRegion.setWidth(checkRegion.width() - CUSHION);
    checkRegion.setHeight(checkRegion.height() -CUSHION);
    checkRegion.moveTopLeft(QPointF(checkRegion.x() + CUSHION/2,checkRegion.y() + CUSHION/2));

    // see if that region is free in the map
    return map()->pathingMap().free(checkRegion);

}

/// Disables the Entity's PathingMap.
///
/// Essentially, clears the region in the Map that corresponds to the Entity's
/// PathingMap.
void Entity::disablePathingMap(){
    QRectF clearRegion;
    clearRegion.setTopLeft(pointPos());
    clearRegion.setWidth(pathingMap().width());
    clearRegion.setHeight(pathingMap().height());
    std::vector<Node> cellsInRegion = map()->pathingMap().cells(clearRegion);
    for (Node cell:cellsInRegion){
        map()->pathingMap().unfill(cell);
    }

    // TODO test remove
    map()->drawPathingMap();
}

/// Enables the Entity's PathingMap.
///
/// Essentially, "draws" the PathingMap of the Entity on the Map's PathingMap.
void Entity::enablePathingMap(){
    map()->pathingMap().setFilling(pointPos(),pathingMap());

    // TODO test, remove
    map()->drawPathingMap();
}

std::vector<Entity *> Entity::children() const
{
    return children_;
}

void Entity::setParentEntity(Entity *parent)
{
    parent_ = parent;
    parent->children_.push_back(this);

    // make sprite set parent too
    sprite()->setParentItem(parent->sprite());
}

/// Maps a point from local (Entity) coordinates to the Map (scene) coordinates.
QPointF Entity::mapToMap(const QPointF &point) const
{
    return sprite()->mapToScene(point);
}

/// Names the specified point (so it can be retrieved with a name).
void Entity::addNamedPoint(const QPointF &point, std::string name)
{
    namedPoints_[name] = point;
}

/// Returns the point represented by the specified name.
QPointF Entity::namedPoint(std::string name)
{
    // make sure the points exists
    assert(namedPoints_.count(name));

    return namedPoints_[name];
}

/// Moves the Entity 1 step closer to its movement in response to keys.
void Entity::moveStepWithKeys()
{
    // TODO, a lot of repeate code, this can be factored out somewhere.

    bool wPressed = map()->game()->keysPressed().count(Qt::Key_W);
    bool sPressed = map()->game()->keysPressed().count(Qt::Key_S);
    bool aPressed = map()->game()->keysPressed().count(Qt::Key_A);
    bool dPressed = map()->game()->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        QLineF line(pointPos(),QPoint(0,0));
        line.setAngle(360-facingAngle());
        line.setLength(stepSize());
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (canFit(newPt)){
            setPointPos(newPt);

            movingWithKeys_ = true;

            // if the walk animation isn't playing already, play it.
            if (sprite()->playingAnimation() != std::string("walk")){
                sprite()->play("walk",-1,100);
            }
        }

    }

    // move down if S is pressed
    if (sPressed){
        QLineF line(pointPos(),QPoint(0,0));
        line.setAngle(360-facingAngle());
        line.setLength(stepSize());
        line.setAngle(line.angle()+180);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (canFit(newPt)){
            setPointPos(newPt);

            movingWithKeys_ = true;

            // if the walk animation isn't playing already, play it.
            if (sprite()->playingAnimation() != std::string("walk")){
                sprite()->play("walk",-1,100);
            }
        }
    }

    // move left if A is pressed
    if (aPressed){
        QLineF line(pointPos(),QPoint(0,0));
        line.setAngle(360-facingAngle());
        line.setLength(stepSize());
        line.setAngle(line.angle()+90);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (canFit(newPt)){
            setPointPos(newPt);

            movingWithKeys_ = true;

            // if the walk animation isn't playing already, play it.
            if (sprite()->playingAnimation() != std::string("walk")){
                sprite()->play("walk",-1,100);
            }
        }

    }

    // move right if D is pressed
    if (dPressed){
        QLineF line(pointPos(),QPoint(0,0));
        line.setAngle(360-facingAngle());
        line.setLength(stepSize());
        line.setAngle(line.angle()-90);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (canFit(newPt)){
            setPointPos(newPt);

            movingWithKeys_ = true;

            // if the walk animation isn't playing already, play it.
            if (sprite()->playingAnimation() != std::string("walk")){
                sprite()->play("walk",-1,100);
            }
        }

    }

    // if none of the keys are pressed, play stand animation
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        // only play if it isn't already playing
        if (sprite()->playingAnimation() != std::string("stand")){
            sprite()->play("stand",-1,100);
        }

        movingWithKeys_ = false;
    }

}
