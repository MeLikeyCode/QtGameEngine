#ifndef FOGWEATHER_H
#define FOGWEATHER_H

#include <QObject>
#include "WeatherEffect.h"

class QTimer;
class QGraphicsPixmapItem;

class FogWeather : public WeatherEffect
{
    Q_OBJECT
public:
    FogWeather();
    ~FogWeather();

    virtual void start();
    virtual void stop();

public slots:
    void fogStep_();

private:
    QTimer* fogTimer_;
    QGraphicsPixmapItem* fog1_;
    QGraphicsPixmapItem* fog2_;
    bool started_;
    bool initial_;
    double maxFogOpacity_;
};

#endif // FOGWEATHER_H
