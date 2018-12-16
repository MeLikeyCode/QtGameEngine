#include "QtUtilities.h"

namespace qge{

namespace QtUtils {

    /// Creates a QPixmap of the specified size filled with the specified color
    QPixmap qPixmapFromColor(QSize size, QColor color)
    {
        QImage img(size,QImage::Format_ARGB32);
        img.fill(color);
        return QPixmap::fromImage(img);
    }

    /// Returns the distance between the two points.
    double distance(QPointF p1, QPointF p2)
    {
        double deltaX = p2.x() - p1.x();
        double deltaY = p2.y() - p1.y();
        return qAbs(qSqrt(qPow(deltaX,2) + qPow(deltaY,2)));
    }

}

}
