#pragma once

#include "Vendor.h"

#include "WeatherEffect.h"
#include "QVector2D"

class QTimer;
class QGraphicsPixmapItem;

namespace qge{

/// A WeatherEffect that represents fog.
/// @see WeatherEffect
/// @author Abdullah Aghazadah
/// @date 6/27/16
class FogWeather : public WeatherEffect
{
    Q_OBJECT
public:
    FogWeather(QPixmap tileableFogGraphic = QPixmap(":/resources/graphics/effects/fog.png"),
                int tileWidth = 1024,
                int tileHeight = 1024,
                double initialOpacity = 0.05,
                double finalOpacity = 0.5,
                double opacityFadeTimeMs = 2000,
                double opacityStepSize = 0.005);
    ~FogWeather();

    virtual void start_() override;
    virtual void stop_() override;
    virtual void resume_() override;
    virtual void pause_() override;

    void setFogSpeed(double pixelsPerSecond);
    void setFogStepSize(double numPixels);

public slots:
    void opacityStep_();
    void moveStep_();

private:
    QTimer* opacityTimer_;
    QTimer* moveTimer_;
    std::set<QGraphicsPixmapItem*> fogSquares_;
    int fogPictureWidth_;
    int fogPictureHeight_;

    // options
    double initialOpacity_;
    double maxOpacity_;
    int opacityFadeTime_; // how long it takes for the opacity to go from initial to max (in ms)
    double opacityStepSize_; // granularity of opacity fading in
    double fogSpeed_; // how fast the fog moves in pixels per second
    double fogStepSize_; // granularity of movement for the fog
    QVector2D fogDirection_; // the direction the fog moves in
    QPixmap fogPicture_;

    // helper functions
    void startTimers_();

    // helper variables
    QRectF fogBoundry_;
    double currentOpacity_;
};

}
