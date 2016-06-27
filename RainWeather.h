#ifndef RAINWEATHER_H
#define RAINWEATHER_H

#include "Weather.h"
#include <QGraphicsPixmapItem>

class Game;
class QTimer;

/// Class that represents rain behavior in a Game.
/// @author Abdullah Aghazadah
/// @date 6/26/16
class RainWeather : public QObject, public Weather
{
    Q_OBJECT
public:
    RainWeather(Game* game);
    ~RainWeather();

    virtual void start();
    virtual void stop();

public slots:
    void rainStep_();
    void splashStep_();
private:
    Game* game_;
    QTimer* rainTimer_;
    QTimer* splashTimer_;
    std::vector<QGraphicsPixmapItem*> rains_;
};

#endif // RAINWEATHER_H
