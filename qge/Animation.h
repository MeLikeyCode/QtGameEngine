#pragma once

#include "Vendor.h"

class QTimer;
class QPixmap;

namespace qge{

class SpriteSheet;
class Node;

/// A QGraphicsItem that represents a series of images that will be shown one after the other at a
/// certain speed ("frames per second").
/// @author Abdullah Aghazadah
/// @date 3/26/17
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Animation* animation = new Animation(spriteSheet,fromNode,toNode); // create an animation with the specified frames of a SpriteSheet
/// animation->play(5,15); // play the animation 5 times at 15 frames per second
/// animation->pause(); // pause the animation at its current frame
/// animation->play(-1,15); // play the animation an infinite number of times
/// connect(animation, &Animation::finished, myObject,myCallback); // be notified each time the animation finishes
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Implementation Details
/// ======================
/// Animation is a QGraphicsItem so that it can go inside a QGraphicsScene. It has a child QGraphicsPixmapItem whos
/// image is constantly changed when play() is called.
class Animation :
        public QObject,         // inherits from QObject so it can use signals/slots
        public QGraphicsItem    // inherits from QGraphicsItem so it can go in a scene
{
    Q_OBJECT
public:
    // constructors
    Animation(QGraphicsItem* parent = nullptr);
    Animation(const QPixmap& pixmap, QGraphicsItem* parent = nullptr);
    Animation(const SpriteSheet& spriteSheet, Node from, Node to, QGraphicsItem* parent = nullptr);

    // modifiers
    void addFrame(const QPixmap& pixmap);
    void addFrames(const SpriteSheet& fromSpriteSheet, const Node& startCell, const Node& endCell);
    void addFrames(std::string folderPath, int numOfImages, std::string imagePrefix, std::string fileExtension);
    void play(int numTimesToPlay, double framesPerSecondToPlayAt);
    void pause();

    // getters
    virtual QRectF boundingRect() const;
    QPixmap currentFrame();
    bool isPlaying();

public slots:
    void animationStep_();

signals:
    /// Emitted when the Animation finished playing the last frame.
    /// If the Animation was asked to play an infinte number of times, this
    /// signal will emit each and every time it plays the last frame.
    void finished(Animation* animation);

private:
    std::vector<QPixmap> pixmaps_;      // all pixmaps of the animation
    QGraphicsPixmapItem* currentPixmap_; // currently displayed pixmap

    QTimer* timer_;
    bool isPlaying_;
    int currentFrame_;
    int timesPlayed_;
    int timesToPlay_;
    double framesPerSecond_;

    void initialize_();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

}
