#ifndef SNOWWEATHER_H
#define SNOWWEATHER_H

#include "Weather.h"
#include <vector>
#include <QObject>

class QGraphicsPixmapItem;
class QTimer;

/// Class that represents snow weather in a Map.
/// @author Abdullah Aghazadah
/// @date 7/2/16
class SnowWeather : public QObject, public Weather
{
    Q_OBJECT
public:
    SnowWeather();
    ~SnowWeather();

    virtual void start();
    virtual void stop();

public slots:
    void snowStep_();
private:
    QTimer* snowTimer_;
    std::vector<QGraphicsPixmapItem*> snows_;
    bool started_;
    bool initial_;
};

#endif // SNOWWEATHER_H
