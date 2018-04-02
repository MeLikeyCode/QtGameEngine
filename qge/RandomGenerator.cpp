#include "RandomGenerator.h"

#include <ctime> // time()
#include <cstdlib> // srand() and rand()
#include <cmath> // fmod (float modulus - in c/c++ '%' is only for integral types)
#include <QPointF>
#include <QRectF>

using namespace qge;

RandomGenerator::RandomGenerator()
{
    // seed
    srand(time(0));
}

int RandomGenerator::randInt(int min, int max)
{
    int range = max - min + 1;
    int r = rand() % range + min;
    return r;
}

int RandomGenerator::randDouble(double min, double max)
{
    double r = (double)rand() / (double)RAND_MAX; // random double between 0 - 1;
    double amountToAdd = (max - min) * r; // based on above, add something to min
    return min + amountToAdd;
}

QPointF RandomGenerator::randQPointF(const QPointF &topLeft, const QPointF &botRight)
{
    double rx = randDouble(topLeft.x(), botRight.x());
    double ry = randDouble(topLeft.y(), botRight.y());
    return QPointF(rx,ry);
}

QPointF RandomGenerator::randQPointF(const QRectF &inRegion)
{
    return randQPointF(inRegion.topLeft(),inRegion.bottomRight());
}
