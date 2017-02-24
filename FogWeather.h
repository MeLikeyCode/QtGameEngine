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

    virtual void start_();
    virtual void stop_();

public slots:
    void opacityStep_();
    void moveStep_();

private:
    QTimer* opacityTimer_;
    QTimer* moveTimer_;
    QGraphicsPixmapItem* fog1_;
    QGraphicsPixmapItem* fog2_;
    bool initial_;
    double initialOpacity_;
    double maxOpacity_;

    // helper functions
    void createGraphicsIfNeeded_();
};

#endif // FOGWEATHER_H
