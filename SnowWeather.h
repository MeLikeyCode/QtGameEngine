#ifndef SNOWWEATHER_H
#define SNOWWEATHER_H

#include "WeatherEffect.h"
#include <vector>
#include <QObject>

class QGraphicsPixmapItem;
class QTimer;

/// Class that represents snow weather in a Map.
/// @author Abdullah Aghazadah
/// @date 7/2/16
class SnowWeather : public WeatherEffect
{
    Q_OBJECT
public:
    SnowWeather();
    ~SnowWeather();

    virtual void start();
    virtual void stop();

public slots:
    void snowStepGlobular_();
    void snowStepLinear_();
private:
    QTimer* globularSnowTimer_;
    QTimer* linearSnowTimer_;
    std::vector<QGraphicsPixmapItem*> globularSnows_;
    QGraphicsPixmapItem* snow1_;
    QGraphicsPixmapItem* snow2_;
    bool started_;
    bool initialGlobular_;
    bool initialLinear_;
};

#endif // SNOWWEATHER_H
