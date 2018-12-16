#pragma once

#include "Vendor.h"

class QPointF;
class QRectF;

namespace qge{

/// Allows generating random numbers/points.
/// Seeds in the constructor.
/// Value semantics - create, use, let die (cheap).
/// @author Abdullah Aghazadah
/// @date 11/25/17
class RandomGenerator
{
public:
    RandomGenerator();

    int randInt(int min, int max);
    int randDouble(double start, double end);
    QPointF randQPointF(const QPointF& topLeft, const QPointF& botRight);
    QPointF randQPointF(const QRectF& inRegion);
};

}
