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
    void snowStepGlobular_();
    void snowStepDuel_();
private:
    QTimer* snowTimer_;
    QTimer* snowTimerDuel_;
    std::vector<QGraphicsPixmapItem*> snows_;
    QGraphicsPixmapItem* snow1_;
    QGraphicsPixmapItem* snow2_;
    bool started_;
    bool initial_;
    bool initialDuel_;
};

#endif // SNOWWEATHER_H
