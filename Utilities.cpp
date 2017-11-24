/// This file defines Utilities.h
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <QtMath>

#include "Map.h"
#include "RandomImageEntity.h"
#include "PathingMap.h"
#include "EntitySprite.h"

/// Adds the specified number of trees randomly scattered on the specified Map.
void addRandomTrees(Map *mapToAddTreesTo, int numTreesToAdd)
{
    for (int i = 0, n = numTreesToAdd; i < n; i++){
        PathingMap pathingMap(1,1,32);
        pathingMap.fill();

        RandomImageEntity* tree = new RandomImageEntity(":/resources/graphics/tree","tree",3,pathingMap);
        mapToAddTreesTo->addEntity(tree);

        double randXPos = rand() % mapToAddTreesTo->width() - 100;
        double randYPos = rand() % mapToAddTreesTo->height() - 100;
        tree->setPos(QPointF(randXPos,randYPos));
    }
}

/// Adds the specified number of rocks randomly scattered on the specified Map.
void addRandomRocks(Map *mapToAddRocksTo, int numRocksToAdd)
{
    for (int i = 0, n = numRocksToAdd; i < n; i++){
        PathingMap pathingMap(1,1,32);
        pathingMap.fill();

        RandomImageEntity* rock = new RandomImageEntity(":/resources/graphics/rock","rock",3,pathingMap);
        mapToAddRocksTo->addEntity(rock);

        double randXPos = rand() % mapToAddRocksTo->width() - 100;
        double randYPos = rand() % mapToAddRocksTo->height() - 100;
        rock->setPos(QPointF(randXPos,randYPos));
    }
}

/// Adds the specified number of bushes randomly scattered on the specified Map.
void addRandomBushes(Map *mapToAddBushesTo, int numBushesToAdd)
{
    for (int i = 0, n = numBushesToAdd; i < n; i++){
        PathingMap pathingMap(1,1,32);
        pathingMap.fill();

        RandomImageEntity* rock = new RandomImageEntity(":/resources/graphics/bush","bush",4,pathingMap);
        mapToAddBushesTo->addEntity(rock);

        double randXPos = rand() % mapToAddBushesTo->width() - 100;
        double randYPos = rand() % mapToAddBushesTo->height() - 100;
        rock->setPos(QPointF(randXPos,randYPos));
    }
}

/// Returns the distance between the two points.
double distance(QPointF p1, QPointF p2)
{
    double deltaX = p2.x() - p1.x();
    double deltaY = p2.y() - p1.y();
    return qAbs(qSqrt(qPow(deltaX,2) + qPow(deltaY,2)));
}

/// Given a step size and a target rate, will tell you the frequency needed to get the
/// target rate.
/// I.e. if my character moves 10 pixels at a time (stepSize) and I want him to move
/// 50 pixels per second (targetRate), how often (in seconds) should I move him?
/// I.e. if my character rotates 10 degrees at a time (stepSize) and I want him to
/// rotate at 50 degrees per second, how often (in seconds) should I move him?
/// Obviously, you need to make sure you are consistent in your units.
double frequency(double stepSize, double targetRate)
{
    // stepFrequency = stepSize / targetRate

    double stepFrequency = stepSize / targetRate;
    return stepFrequency;
}

/// Converts seconds to milliseconds.
double secondsToMs(double seconds)
{
    static const int MS_IN_S = 1000;

    double ms = seconds * MS_IN_S;
    return ms;
}

/// Converts milliseconds to seconds.
double msToSeconds(double milliseconds)
{
    return milliseconds/1000.0;
}


/// Generates a random 32 bit number (unsigned).
/// This function is a lot more inexpensive than rand() but also not as reliable
/// (i.e. for scientific purposes).
quint32 fastRandom()
{
    static quint32 num = 7;

    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

    return num;
}

/// Returns the angle in 'allAngles' that is the closest to the 'targetAngle'.
/// Angles are measured from 0-360 and "wrap around".
double closestAngle(const std::vector<int> &allAngles, double targetAngle)
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
