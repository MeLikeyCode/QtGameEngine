#pragma once

#include "Vendor.h"

#include "WeatherEffect.h"

class QTimer;
class QGraphicsPixmapItem;

namespace qge{

class Sound;

/// Class that represents a rain weather effect.
/// @see WeatherEffect
/// @author Abdullah Aghazadah
/// @date 6/26/16
class RainWeather : public WeatherEffect
{
    Q_OBJECT
public:
    RainWeather(QPixmap rainGraphic = QPixmap(":/resources/graphics/effects/rain.png") ,
                int numOfRains=150,
                int rainFalldownSpeed=1500,
                int rainMoveAmountPerStep=100,
                int splashStepFreqMs=150,
                int numSplashPerStep=7,
                double rainInitialOpacity = 0.08,
                double rainMaxOpacity = 0.5,
                double rainOpacityStepSize = 0.005,
                int rainInitialToMaxOpacityTimeMS = 5000,
                double splashInitialOpacity = 0.08,
                double splashFinalOpacity = 0.5,
                double splashOpacityStepSize = 0.002,
                int splashInitialToFinalOpacityTimeMS = 5000);
    ~RainWeather();

    virtual void start_() override;
    virtual void stop_() override;
    virtual void resume_() override;
    virtual void pause_() override;

public slots:
    void rainMoveStep_();
    void rainOpacityStep_();
    void createSplashesStep_();
    void splashOpacityStep_();
private:
    QTimer* rainMoveTimer_;
    QTimer* rainOpacityTimer_;
    QTimer* createSplashTimer_;
    QTimer* splashOpacityTimer_;
    std::vector<QGraphicsPixmapItem*> rains_;
    Sound* rainSound_;

    // options
    QPixmap rainGraphic_;       // the graphic to use for the actual rain
    int numOfRainGraphics_;        // number of rain graphics to randomly generate
    int fallDownSpeed_;              // speed at which the rain graphics fall down (in pixels per second)
    int rainStepSize_;          // how much to move the rain down every step (determines movement granularity)
    int splashStepFreq_;           // how often to create a group of splashes (in ms)
    int numSplashPerStep_;          // how many splashes to create each time they are created
    double rainInitialOpacity_;
    double rainMaxOpacity_;
    double rainOpacityStepSize_; // granularity to increase the opacity of the rain in
    int rainInitialToMaxOpacityTime_; // how long do we have to go from rains initial to max opacity
    double splashInitialOpacity_;
    double splashMaxOpacity_;
    double splashOpacityStepSize_;
    int splashInitialToMaxOpacityTime_;

    // helper function
    void startTimers_();

    double currentRainOpacity_;
    double currentSplashOpacity_;
};

}
