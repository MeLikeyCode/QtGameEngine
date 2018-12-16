/// This file defines Utilities.h
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

#include "Utilities.h"

#include "Map.h"
#include "RandomImageEntity.h"
#include "PathingMap.h"
#include "EntitySprite.h"
#include "RandomGenerator.h"
#include "SpriteSheet.h"
#include "AngledSprite.h"

using namespace qge;

/// Adds the specified number of trees randomly scattered on the specified Map.
void qge::addRandomTrees(Map *mapToAddTreesTo, int numTreesToAdd, const std::string& treeNumber, int numImages)
{
    for (int i = 0, n = numTreesToAdd; i < n; i++){
        std::string pmPath;
        if (std::string(treeNumber) == std::string("One"))
            pmPath = "treeOnepathing";
        if (std::string(treeNumber) == std::string("Two"))
            pmPath = "treeTwopathing";
        if (std::string(treeNumber) == std::string("Three"))
            pmPath = "treeThreepathing";
        if (std::string(treeNumber) == std::string("Four"))
            pmPath = "treeFourpathing";
        std::string fullp = ":/resources/graphics/tree/" + pmPath + ".png";
        PathingMap* pm = new PathingMap(QPixmap(fullp.c_str()),32);

        RandomImageEntity* tree = new RandomImageEntity(":/resources/graphics/tree","tree" + treeNumber,numImages,*pm);
        tree->addTag("scenery");
        mapToAddTreesTo->addEntity(tree);

        double randXPos = randDouble(100,mapToAddTreesTo->width()-100);
        double randYPos = randDouble(100,mapToAddTreesTo->height()-100);
        tree->setPos(QPointF(randXPos,randYPos));
    }
}


/// Given a step size and a target rate, will tell you the frequency needed to get the
/// target rate.
/// I.e. if my character moves 10 pixels at a time (stepSize) and I want him to move
/// 50 pixels per second (targetRate), how often (in seconds) should I move him?
/// I.e. if my character rotates 10 degrees at a time (stepSize) and I want him to
/// rotate at 50 degrees per second, how often (in seconds) should I move him?
/// Obviously, you need to make sure you are consistent in your units.
double qge::frequency(double stepSize, double targetRate)
{
    // stepFrequency = stepSize / targetRate

    double stepFrequency = stepSize / targetRate;
    return stepFrequency;
}

/// Converts seconds to milliseconds.
double qge::secondsToMs(double seconds)
{
    static const int MS_IN_S = 1000;

    double ms = seconds * MS_IN_S;
    return ms;
}

/// Converts milliseconds to seconds.
double qge::msToSeconds(double milliseconds)
{
    return milliseconds/1000.0;
}


/// Generates a random 32 bit number (unsigned).
/// This function is a lot more inexpensive than rand() but also not as reliable
/// (i.e. don't use for scientific purposes).
quint32 qge::fastRandom()
{
    static quint32 num = 7;

    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

    return num;
}

/// Returns the angle in 'allAngles' that is the closest to the 'targetAngle'.
/// Angles are measured from 0-360 and "wrap around".
double qge::closestAngle(const std::vector<int> &allAngles, double targetAngle)
{
    assert(!allAngles.empty());

    targetAngle = fmod(targetAngle,360);

    if (targetAngle < 0)
        targetAngle = 360 - abs(targetAngle);

    double closest = allAngles[0]; // assume first angle is the closest and search for better
    double closestDist1 = 360-closest+targetAngle;
    double closestDist2 = abs(targetAngle-closest);
    double closestNearestDist = std::min(closestDist1,closestDist2);
    for (int angle : allAngles){
        double dist1 = 360-angle+targetAngle;
        double dist2 = abs(targetAngle-angle);
        double nearest = std::min(dist1,dist2);
        if (nearest < closestNearestDist){
            closest = angle;
            closestNearestDist = nearest;
        }
    }
    return closest;
}

double qge::randInt(int from, int to)
{
    static RandomGenerator generator;

    return generator.randInt(from,to);
}

double qge::randDouble(double from, double to)
{
    static RandomGenerator generator;

    return generator.randDouble(from,to);
}

/// Adds a tag to all the specified entities.
void qge::addTag(const std::string &tag, std::initializer_list<Entity *> entities)
{
    for (Entity* entity:entities)
        entity->addTag(tag);
}

/// Convenience function which creates an entity and gives it the built in minitaur sprite.
Entity *qge::getMinitaurEntity()
{
    qge::Entity* entity = new qge::Entity();

    qge::SpriteSheet minitaurSpriteSheet(":/resources/graphics/characterSpritesheets/minotaur_alpha.png", 24, 8, 128, 128);

    qge::AngledSprite* entitySprite = new qge::AngledSprite();

    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        entitySprite->addFrames((180+45*i) % 360, "stand", minitaurSpriteSheet, qge::Node(0,0+i), qge::Node(3,0+i));
        for (int j = 2; j > 0; --j){
            entitySprite->addFrame(minitaurSpriteSheet.tileAt(qge::Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        entitySprite->addFrames((180+45*i) % 360,"walk",minitaurSpriteSheet,qge::Node(4,0+i),qge::Node(11,0+i));
        // attack
        entitySprite->addFrames((180+45*i) % 360,"attack",minitaurSpriteSheet,qge::Node(12,0+i),qge::Node(18,0+i));
    }

    entity->setSprite(entitySprite);

    return entity;
}

Entity *qge::getSpiderEntity()
{
    qge::Entity* spiderEntity = new qge::Entity();

    qge::SpriteSheet spiderSpriteSheet(":/resources/graphics/characterSpritesheets/spider.png",48,8,128,128);

    qge::AngledSprite* spiderSprite = new qge::AngledSprite();
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        spiderSprite->addFrames((180+45*i) % 360,"stand",spiderSpriteSheet,qge::Node(0,0+i),qge::Node(3,0+i));
        for (int j = 2; j > 0; --j){
            spiderSprite->addFrame(spiderSpriteSheet.tileAt(qge::Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        spiderSprite->addFrames((180+45*i) % 360,"walk",spiderSpriteSheet,qge::Node(4,0+i),qge::Node(11,0+i));

        // attack
        spiderSprite->addFrames((180+45*i) % 360,"attack",spiderSpriteSheet,qge::Node(12,0+i),qge::Node(15,0+i));

        // hit (animation that will play when the spider is hit - but not killed)
        spiderSprite->addFrames((180+45*i) % 360,"hit",spiderSpriteSheet,qge::Node(18,0+i),qge::Node(20,0+i));

        // die (animation that will play when spider dies)
        spiderSprite->addFrames((180+45*i) % 360,"die",spiderSpriteSheet,qge::Node(18,0+i),qge::Node(23,0+i));

        // dieTwo (a second animation that might play when spider dies - just adds variety)
        spiderSprite->addFrames((180+45*i) % 360,"dieTwo",spiderSpriteSheet,qge::Node(24,0+i),qge::Node(31,0+i));
    }

    spiderEntity->setSprite(spiderSprite);
    spiderEntity->sprite()->play("stand",-1,15,0);

    return spiderEntity;
}
