#ifndef QTUTILITIES_H
#define QTUTILITIES_H

class QPixmap;
class QImage;
class QColor;
class QSize;
class QPointF;
class QRectF;

/// This file contains a bunch of utility functions that make it easier to use
/// some Qt features.
/// @author Abdullah Aghazadah
/// @date 10-11-16
/// @file

namespace QtUtils {
    QPixmap qPixmapFromColor(QSize size, QColor color);
    double distance(QPointF p1, QPointF p2);
}





#endif // QTUTILITIES_H
