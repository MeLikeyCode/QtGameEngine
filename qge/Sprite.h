#pragma once

#include "Vendor.h"

#include "PlayingAnimationInfo.h"

class QGraphicsPixmapItem;
class QTimer;

namespace qge{

class SpriteSheet;
class Node;

/// A QGraphicsItem that represents a bunch of animations that can be played.
/// @author Abdullah Aghazadah
///
/// Since a Sprite is a QGraphicsItem, it can be placed inside a QGraphicsScene. To add a frame to
/// an animation, use Sprite::addFrame(QPixmap,std::string). The specified pixmap will then be
/// added to the specified animation. If an animation of that name does not exist, it will be
/// created. If an animation of that name already exists, then the pixmap will be added as the next
/// image in the animation. To add a bunch of images from a folder to an animation use
/// Sprite::addFrames(std::string,int,std::string,std::string). You can also add tiles from a
/// SpriteSheet as animation frames in the Sprite.
///
/// A Sprite can play an animation a certain number of times by using Sprite::play(). A value of -1
/// times means that the animation will be played again and again forever.
class Sprite:public QObject, // inherits from QObject so it can use signals/slots
             public QGraphicsItem{ // inherits from QGraphicsItem so it can go inside scenes
    Q_OBJECT
public:
    // constructors
    Sprite(QGraphicsItem* parent=nullptr);
    Sprite(QPixmap pixmap, QGraphicsItem* parent=nullptr);

    // readers
    bool hasAnimation(std::string animation) const;
    std::vector<std::string> animations() const;
    PlayingAnimationInfo playingAnimation() const;
    QPixmap currentFrame() const;
    int currentFrameNumber() const;

    // modifiers
    void addFrame(QPixmap frame, std::string toAnimation);
    void addFrame(const Node& tile, const SpriteSheet& fromSpriteSheet, std::string toAnimation);
    void addFrames(std::string folder, int numOfImages, const std::string& imagePrefix, const std::string& toAnimation);
    void addFrames(const Node& topLeftCell, const Node& bottomRightCell, const SpriteSheet& fromSpriteSheet, std::string toAnimation);
    void setPixmap(QPixmap pixmap);

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual QRectF boundingRect() const;

    void play(std::string animation, int timesToPlay, double framesPerSecond, int startingFrameNumber = 0);
    void playThenGoBackToOldAnimation(std::string animation, int numTimesToPlay, double framesPerSecond, int startingFrameNumber = 0);
    void stop();

public slots:
    void nextFrame_();
    void onTemporaryPlayDone_(Sprite* sender, std::string animation);

signals:
    /// Emitted each time the frame switches due to an animation playing.
    void frameSwitched(Sprite* sender, int fromFrameNumber, int toFrameNumber);

    /// Emitted each time an animation has finished playing.
    /// Even if an animation was asked to play multiple (even infinite) number of times,
    /// this event will emit every time the last frame is reached.
    void animationFinished(Sprite* sender, std::string animation);

    /// Emitted when an animation has finished playing the number of times it was asked to play.
    /// The difference between this and animationFinished() is that this is only
    /// emitted when the animation has finished playing the number of times
    /// it was asked to play, where as animationFinished() will be emitted
    /// every single time the last frame is reached.
    void animationFinishedCompletely(Sprite* sender, std::string animation);

private:
    // mapping of string : vector of pixmaps (an animation)
    std::unordered_map<std::string,std::vector<QPixmap>> animation_;
    std::string playingAnimation_;
    int playingAnimationFPS_; // the fps the currently playing animation is playing at

    QGraphicsPixmapItem* pixmapItem_; // currently displayed pixmap
    QTimer* timer_;
    std::vector<QPixmap> animationPixmaps; // frames of the currently
                                           // playing animation
    int currentFrame_;
    int timesPlayed_;
    int timesToPlay_;

    void commonInitialize_();
    void setSize_(std::string ofAnimation, int width, int height);
    int playingAnimationTimesLeftToPlay_() const;

    PlayingAnimationInfo animationPlayingLast_;
};

}
