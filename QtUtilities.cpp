#include "QtUtilities.h"
#include <QPixmap>
#include <QImage>
#include <QColor>

/// Creates a QPixmap of the specified size filled with the specified color
QPixmap qPixmapFromColor(QSize size, QColor color)
{
    QImage img(size,QImage::Format_RGB32);
    img.fill(color);
    return QPixmap::fromImage(img);
}
