#ifndef RAINWEATHER_H
#define RAINWEATHER_H

#include "WeatherEffect.h"
#include <QObject>

class QTimer;
class QGraphicsPixmapItem;

/// Class that represents a rain weather effect in a Map;
/// @see WeatherEffect
/// @author Abdullah Aghazadah
/// @date 6/26/16
class RainWeather : public QObject, public WeatherEffect
{
    Q_OBJECT
public:
    RainWeather(int numOfRainGraphics=45, int rainStepFreqMs=10, int rainMoveAmountPerStep=100,
                int splashStepFreq=75, int numSplashPerStep=7);
    ~RainWeather();

    virtual void start();
    virtual void stop();

public slots:
    void rainStep_();
    void splashStep_();
private:
    QTimer* rainTimer_;
    QTimer* splashTimer_;
    std::vector<QGraphicsPixmapItem*> rains_;
    bool started_;
    double currentSplashOpacity_;

    int numOfRainGraphics_ = 25;        // number of rain graphics
    int rainStepFreqMs_ = 10;           // how often to move the rain down in ms
    int rainMoveAmountPerStep_ = 100;   // how much to move the rain down every step
    int splashStepFreq_ = 75;           // how often to create splashes
    int numSplashPerStep_ = 7;          // how many splashes to create each time they are created
};

#endif // RAINWEATHER_H
