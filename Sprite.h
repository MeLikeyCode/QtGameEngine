#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <QGraphicsItem>
#include <QObject>

#include <unordered_map>
#include <vector>
#include <QPixmap>

class QGraphicsPixmapItem;
class QTimer;

/// A QGraphicsItem that represents a sprite with named animations that can
/// be played.
/// @author Abdullah Aghazadah
/// @date 5-16-15
///
/// Since a Sprite is a QGraphicsItem, it can be placed inside a
/// QGraphicsScene. To add a QPixmap to an animation, use
/// Sprite::addFrame(QPixmap,std::string). The specified pixmap will then
/// be added to the specified animation. If an animation of that name does not
/// exist, it will be created. If an animation of that name already exists,
/// then the pixmap will be added as the next image in the animation. To add a
/// bunch of images from a resource folder to an animation use
/// Sprite::addFrames(std::string,int,std::string).
///
/// A Sprite can play an animation a certain number of times by using
/// Sprite::play(std::string,int,int). A value of -1 times means that the
/// animation will be played again and again forever.
class Sprite:public QObject, public QGraphicsItem{
    Q_OBJECT
public:
    // constructor
    Sprite(QGraphicsItem* parent=nullptr);
    Sprite(QPixmap pixmap, QGraphicsItem* parent=nullptr);

    // readers
    QSizeF size();
    virtual QRectF boundingRect() const;
    bool hasAnimation(std::string animation) const;
    std::string playingAnimation();
    QPixmap currentFrame() const;

    // modifiers
    void addFrame(QPixmap frame, std::string toAnimation);
    void addFrames(std::string resourceFolder, int numOfImages, std::string imagePrefix);
    void setPixmap(QPixmap pixmap);
    void stop();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void play(std::string animation, int timesToPlay, int delayBetweenFrames);

    // resizing
    void setSize(std::string animation, int width, int height);
    void setSize(int width, int height);
    void setSizeOfCurrentFrame(int width, int height);

public slots:
    void nextFrame_();

signals:
    // emitted each time an animation has finished playing
    void animationFinished(Sprite* sprite, std::string animation);

private:
    // mapping of string : vector of pixmaps (an animation)
    std::unordered_map<std::string,std::vector<QPixmap>> animation_;
    std::string playingAnimation_;

    QGraphicsPixmapItem* pixmap_; // currently displayed pixmap
    QTimer* timer_;
    std::vector<QPixmap> animationPixmaps; // frames of the currently
                                           // playing animation
    int currentFrame_;
    int timesPlayed_;
    int timesToPlay_;

    QSizeF size_;

};

#endif // SPRITE_H
