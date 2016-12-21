#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>

class Map;

/// Abstract class that represents a weather effect for a Map. To add a
/// WeatherEffect effect to a Map, first create an instance of the
/// WeatherEffect, then use Map::setWeatherEffect(WeatherEffect*).
///
/// To create your own weather effect, extend this class and define what
/// happens to the Map when the weather effect is started (virtual void
/// start()) and stopped (virtual void stop()).
///
/// @author Abdullah Aghazadah
/// @date 6/26/16
class WeatherEffect: public QObject // so that we can use QPointer<WeatherEffect>
{
    Q_OBJECT
    friend class Map; // Map needs to set map_ during Map::setWeather(Weather*)
                      // (because a WeatherEffect operates on a Map)
public:
    WeatherEffect();

    /// Starts the WeatherEffect on the Map. Does nothing if the WeatherEffect
    /// was already started.
    virtual void start() = 0;

    /// Stops the WeatherEffect on the Map. Does nothing if the WeatherEffect
    /// is already stopped.
    virtual void stop() = 0;
protected:
    Map* map_;
};

#endif // WEATHER_H
