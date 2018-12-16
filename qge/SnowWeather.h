#pragma once

#include "Vendor.h"

#include "WeatherEffect.h"

class QGraphicsPixmapItem;
class QTimer;

namespace qge{

/// Class that represents a snow weather effect.
/// @see WeatherEffect
/// @author Abdullah Aghazadah
/// @date 7/2/16
class SnowWeather : public WeatherEffect
{
    Q_OBJECT
public:
    SnowWeather();
    ~SnowWeather();

    virtual void start_() override;
    virtual void stop_() override;
    virtual void resume_() override;
    virtual void pause_() override;

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

    void startTimers_();
};

}
