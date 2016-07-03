#ifndef WEATHER_H
#define WEATHER_H

class Map;

/// Abstract class that represents a weather effect for a Map. To add a
/// WeatherEffect effect to a Map, first create an instance of the
/// WeatherEffect, then use Map::setWeatherEffect(WeatherEffect*).
///
/// To create your own weather effect, extend this class and define what
/// happens to the Map when the weather effect is started (virtual void
/// start()) and stopped (virtual void stop()). Make sure your implementations
/// of start() and stop() first call WeatherEffect::start() and
/// WeatherEffect::stop() respectively. These functions will assert some pre
/// conditions (such as a Map being set).
///
/// @author Abdullah Aghazadah
/// @date 6/26/16
class WeatherEffect
{
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
